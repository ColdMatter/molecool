
#include <omp.h>
#include <mkl.h>
//#include <valarray>
#include <vector>

#include "Log.h"
#include "Ensemble.h"
#include "Distribution.h"

namespace molecool {

	Ensemble::Ensemble(int nParticles, std::array<molecool::Distribution, 6>& dists)
	{
		MC_CORE_TRACE("Initializing an ensemble of {0} particles", nParticles);
		//particles.reserve((size_t)nParticles);	// sets capacity without initializing elements
		particles.resize((size_t)nParticles);		// sets capacity and initializes members
		MC_CORE_TRACE("particles vector has size {0} with capacity {1}", particles.size(), particles.capacity());

		// generate the required number of random numbers
		// TO BE PARALLELIZED
		int seed = (int)time(0);						// current time in seconds as a seed
		vslNewStream(&stream, VSL_BRNG_MT2203, seed); 	// args: stream, generator type, seed
		int rngStatus;
		int nDims = dists.size();						// number of phase-space dimensions
		int nRandoms = nParticles * nDims;

		// vector-approach to initializing the ensemble, cache friendly method
		std::vector<double> target(nRandoms);			// target array for random numbers
		MC_CORE_TRACE("Starting random number generation of {0} doubles", nRandoms);
		for (size_t i = 0; i < nDims; ++i) {
			dists[i].sample(stream, nParticles, (double*)(&target.at(i * nParticles)));
		}
		MC_CORE_TRACE("end random number generation");
		
		MC_CORE_TRACE("Initialization of ensemble started");
		// in memory, target vector now looks like [ ... xs ...  ys ... zs ... vxs ... vys ... vzs ... ]
		// need to populate ensemble, here THE VECTOR WAY, should be the fastest due to low number of cache misses
		// BUT THIS RAW MANIPULATION OF POINTERS TO THE VECTOR CONTENTS (TYPE PUNNING) IS RATHER FRAGILE AND SCARY
		// THIS WORKS BUT SCARES THE HELL OUT OF ME
		for (long i = 0; i < nDims; ++i) {				// distribute phase-space coordinates
			for (long j = 0; j < nParticles; ++j) {
				double* dataPtr = (double*) ((i < nDims/2) ? (&particles[j].position.x) : (&particles[j].velocity.x));
				//*(dataPtr + (j * sizeof(Particle) / sizeof(double) + i)) = target[i * nParticles + j];					// populate ensemble particle coordinate
				*(dataPtr + i) = target[i * (long)nParticles + j];
			}
		}
		MC_CORE_TRACE("Ensemble initialization complete");
		
		/*
		// this is equivalent to the code block just above 
		// it is easier to read and doesn't require using raw pointers or type punning
		// but is 4x slower because the valarray slicing operation is a cache miss on every read
		std::valarray<double> vTarget(nRandoms);
		for (size_t i = 0; i < 6; ++i) {
			dists[i].sample(stream, nParticles, &vTarget[i * nParticles]);				// valarray  version
		}
		*/
		
	}

	Ensemble::~Ensemble() {
		vslDeleteStream(&stream);
	}


}
