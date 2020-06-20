
#include <omp.h>
#include <mkl.h>
//#include <valarray>
#include <vector>

#include "Log.h"
#include "Ensemble.h"
#include "Random.h"
#include "ScopedTimer.h"

namespace molecool {

	Ensemble::Ensemble(int nParticles, std::array<Distribution, 6>& dists)
		: distributions(dists)
	{
		MC_CORE_INFO("Initializing an ensemble of {0} particles", nParticles);
		//particles.reserve((size_t)nParticles);	// sets capacity without initializing elements
		particles.resize((size_t)nParticles);		// sets capacity and initializes members
		MC_CORE_TRACE("particles vector has size {0} with capacity {1}", particles.size(), particles.capacity());

		// generate the required number of random numbers
		MC_CORE_TRACE("Starting random number generation for ensemble");
		int seed = (int)time(0);						// current time in seconds as a seed
		int nDims = dists.size();						// number of phase-space dimensions
		int nRandoms = nParticles * nDims;
		std::vector<double> target(nRandoms);			// target for random numbers
		// For random number generation, make a thread for each phase-space coordinate for simplicity
		// create independent random number streams for each thread before parallel section
		std::vector<std::shared_ptr<RandomStream>> sps;
		for (int i = 0; i < nDims; ++i) {
			sps.push_back(std::make_shared<RandomStream>(seed));
		}
		omp_set_num_threads(nDims);
		#pragma omp parallel for
		for (int i = 0; i < nDims; ++i) {
			dists[i].sample(sps[i], nParticles, (double*)(&target.at(i * nParticles)));	// write to target array	
		}
		MC_CORE_TRACE("End random number generation for ensemble");
		
		MC_CORE_INFO("Starting ensemble initilization");
		mkl_domatcopy('R', 'T', nDims, nParticles, 1, target.data(), nParticles, (double*)particles.data(), nDims);
		MC_CORE_INFO("End ensemble initilization");
		
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
