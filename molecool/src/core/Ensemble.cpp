
#include <omp.h>
#include <mkl.h>
//#include <valarray>
#include <vector>

#include "Log.h"
#include "Ensemble.h"
#include "Random.h"
#include "ScopedTimer.h"

namespace molecool {

	Ensemble::Ensemble(long nParticles, std::array<Distribution, 6>& dists)
		: distributions(dists)
	{
		MC_CORE_INFO("Initializing an ensemble of {0} particles", nParticles);
		//particles.reserve((size_t)nParticles);	// sets capacity without initializing elements
		particles.resize((size_t)nParticles);		// sets capacity and initializes members
		MC_CORE_TRACE("particles vector has size {0} with capacity {1}", particles.size(), particles.capacity());

		// generate the required number of random numbers
		// TO BE PARALLELIZED, last thread can handle any remaining tail that does not divide up evenly
		int seed = (int)time(0);						// current time in seconds as a seed
		int nDims = dists.size();						// number of phase-space dimensions
		int nRandoms = nParticles * nDims;

		// vector-approach to initializing the ensemble, cache friendly but slightly dodgy
		std::vector<double> target(nRandoms);			// target array for random numbers
		MC_CORE_TRACE("Starting random number generation of {0} doubles", nRandoms);

		MC_CORE_INFO("particle object size is {0} bytes", sizeof(Particle));
		
		// single-processor way with a single random stream (simple)
		auto sp = std::make_shared<RandomStream>(seed);	// allocate random number stream
		MC_CORE_INFO("Single processor begin");
		for (size_t i = 0; i < nDims; ++i) {
			dists[i].sample(sp, nParticles, (double*)(&target.at(i * nParticles)));
		}
		MC_CORE_INFO("Single processor end");

		//*/
		// parallel processing way

		// for random number generation, make a thread for each phase-space coordinate for simplicity
		// otherwise, it is a mess where each thread must cross over different distributions
		// create independent random number streams for each thread before parallel section
		// let openmp do the work, hopefully it is clever enough to do it right
		std::vector<std::shared_ptr<RandomStream>> sps;
		for (int i = 0; i < nDims; ++i) {
			sps.push_back(std::make_shared<RandomStream>(seed));
		}
		omp_set_num_threads(nDims);
		MC_CORE_INFO("Multiple processor begin");
		#pragma omp parallel for
		for (int i = 0; i < nDims; ++i) {
			MC_CORE_TRACE("hello from thread {0}", omp_get_thread_num());
			dists[i].sample(sps[i], nParticles, (double*)(&target.at(i * nParticles)));
		}
		MC_CORE_INFO("Multiple processor end");
		//*/

		MC_CORE_TRACE("End random number generation");
		
		MC_CORE_TRACE("Ensemble initialization started");
		// in memory, target vector now looks like [ ... xs ...  ys ... zs ... vxs ... vys ... vzs ... ]
		// need to populate ensemble, this way should be fast due to low number of cache misses
		// BUT THIS RAW MANIPULATION OF POINTERS TO THE VECTOR CONTENTS (TYPE PUNNING) IS RATHER FRAGILE AND SCARY
		for (int i = 0; i < nDims; ++i) {				// distribute phase-space coordinates
			for (long j = 0; j < nParticles; ++j) {
				double* dataPtr = (double*) ((i < nDims/2) ? (&particles[j].position.x) : (&particles[j].velocity.x));
				//*(dataPtr + (j * sizeof(Particle) / sizeof(double) + i)) = target[i * nParticles + j];
				*(dataPtr + i) = target[i * nParticles + j];
			}
		}
		MC_CORE_TRACE("Ensemble initialization complete");
		
		/*
		// this comment block is functionally equivalent to the code block just above
		// it is easier to read and doesn't require using raw pointers or type punning
		// but is ~4x slower because the valarray slicing operation is a cache miss on every read
		std::valarray<double> vTarget(nRandoms);
		for (size_t i = 0; i < nDims; ++i) {
			dists[i].sample(sp, nParticles, &vTarget[i * nParticles]);
		}
		// in memory, target valarray now looks like [ ... xs ...  ys ... zs ... vxs ... vys ... vzs ... ]
		// can slice through it and construct particles to populate the vector container
		for (long i = 0; i < nParticles; ++i) {
			std::valarray<double> subArray = vTarget[std::slice(i, 6, nParticles)];		// std::slice(start, size, stride)
			particles[i] = Particle(subArray);
		}
		*/
		
	}


}
