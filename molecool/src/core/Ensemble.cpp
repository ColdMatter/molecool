
#include <omp.h>
#include <mkl.h>
#include <vector>
#include <stdlib.h>

#include "Log.h"
#include "Ensemble.h"
#include "Random.h"
#include "ScopedTimer.h"

namespace molecool {

	Ensemble::Ensemble(int nParticles, std::array<Distribution, 6>& dists)
		: distributions(dists)
	{
		MC_CORE_INFO("Creating an ensemble of {0} particles", nParticles);
		try {
			MC_CORE_TRACE("Allocating particles vector");
			//particles.reserve((size_t)nParticles);	// sets capacity without initializing elements
			particles.resize((size_t)nParticles);		// sets capacity and initializes members
			MC_CORE_TRACE("Allocation of particles vector finished");
			
			// generate the required number of random numbers
			MC_CORE_TRACE("Generating random numbers for ensemble");
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
			MC_CORE_TRACE("Generation of random numbers finished");

			MC_CORE_INFO("Initializing ensemble particles");
			mkl_domatcopy('R', 'T', nDims, nParticles, 1, target.data(), nParticles, (double*)particles.data(), nDims);
			MC_CORE_INFO("Initialization finished");
		}
		catch (...) {
			MC_CORE_FATAL("Unable to create ensemble (probably insufficient memory) - exiting.");
			exit(-1);
		}
	}


}
