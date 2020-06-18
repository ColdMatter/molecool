
#include <omp.h>
#include <mkl.h>
//#include <valarray>
#include <vector>

#include "Log.h"
#include "Ensemble.h"
#include "Distribution.h"

namespace molecool {

	Ensemble::Ensemble(long nParticles, std::array<molecool::Distribution, 6>& dists)
	{
		MC_CORE_INFO("Initializing an ensemble of {0} particles", nParticles);
		//particles.reserve((size_t)nParticles);	// sets capacity without initializing elements
		particles.resize((size_t)nParticles);		// sets capacity and initializes members
		MC_CORE_TRACE("particles vector has size {0} with capacity {1}", particles.size(), particles.capacity());

		// generate the required number of random numbers
		// TO BE PARALLELIZED, last thread can handle any remaining tail that does not divide up evenly
		int seed = (int)time(0);						// current time in seconds as a seed
		int rngStatus = vslNewStream(&stream, VSL_BRNG_MT2203, seed); 	// args: stream, generator type, seed
		int nDims = dists.size();						// number of phase-space dimensions
		int nRandoms = nParticles * nDims;

		// vector-approach to initializing the ensemble, cache friendly method
		std::vector<double> target(nRandoms);			// target array for random numbers
		MC_CORE_TRACE("Starting random number generation of {0} doubles", nRandoms);
		for (size_t i = 0; i < nDims; ++i) {
			dists[i].sample(stream, nParticles, (double*)(&target.at(i * nParticles)));
		}
		MC_CORE_TRACE("end random number generation");
		
		MC_CORE_TRACE("Ensemble initialization started");
		// in memory, target vector now looks like [ ... xs ...  ys ... zs ... vxs ... vys ... vzs ... ]
		// need to populate ensemble, this way should be fast due to low number of cache misses
		// BUT THIS RAW MANIPULATION OF POINTERS TO THE VECTOR CONTENTS (TYPE PUNNING) IS RATHER FRAGILE AND SCARY
		for (int i = 0; i < nDims; ++i) {				// distribute phase-space coordinates
			for (long j = 0; j < nParticles; ++j) {
				double* dataPtr = (double*) ((i < nDims/2) ? (&particles[j].position.x) : (&particles[j].velocity.x));
				//*(dataPtr + (j * sizeof(Particle) / sizeof(double) + i)) = target[i * nParticles + j];					// populate ensemble particle coordinate
				*(dataPtr + i) = target[(long)i * nParticles + j];
			}
		}
		MC_CORE_TRACE("Ensemble initialization complete");
		
		/*
		// this is functionally equivalent to the code block just above
		// it is easier to read and doesn't require using raw pointers or type punning
		// but is ~4x slower because the valarray slicing operation is a cache miss on every read
		std::valarray<double> vTarget(nRandoms);
		for (size_t i = 0; i < nDims; ++i) {
			dists[i].sample(stream, nParticles, &vTarget[i * nParticles]);
		}
		// in memory, target valarray now looks like [ ... xs ...  ys ... zs ... vxs ... vys ... vzs ... ]
		// can slice through it and construct particles to populate the vector container
		for (long i = 0; i < nParticles; ++i) {
			std::valarray<double> subArray = vTarget[std::slice(i, 6, nParticles)];		// (start, size, stride)
			particles[i] = Particle(subArray);
		}
		*/
		
	}

	Ensemble::~Ensemble() {
		vslDeleteStream(&stream);
	}


}
