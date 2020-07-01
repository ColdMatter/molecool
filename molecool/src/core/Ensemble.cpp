#include "mcpch.h"
#include "Ensemble.h"
#include "Random.h"

namespace molecool {

	Ensemble::Ensemble(int nParticles, Particle p, std::vector< std::pair< Distribution, Distribution> >& dists)
		: particles(nParticles), particle(p), distributions(dists)
	{
		try {
			MC_CORE_INFO("Creating an ensemble of {0} particles of type {1}", particles, particle);
			dimensions = (int)distributions.size();
			assert(dimensions > 0);
			assert(particles > 0);
			MC_CORE_INFO("Dimensionality is {0}", dimensions);
			size_t nRandoms = (size_t)(particles * dimensions);
			positions.resize(nRandoms);		
			velocities.resize(nRandoms);
			
			// generate the required number of random numbers
			MC_CORE_TRACE("Generating random numbers for ensemble states");
			int seed = (int)time(0);						// current time in seconds as seed	
			std::vector<double> tempPositions(nRandoms);	// target for particle positions
			std::vector<double> tempVelocities(nRandoms);	// target for particle velocities
			
			// For random number generation, make a thread for each phase-space coordinate for simplicity
			// create independent random number streams for each thread before parallel section
			int nThreads = 2 * dimensions;
			std::vector<std::shared_ptr<RandomStream>> sps;
			for (int i = 0; i < nThreads; ++i) {
				sps.push_back(std::make_shared<RandomStream>(seed));
			}
			omp_set_num_threads(nThreads);
			#pragma omp parallel for
				for (int i = 0; i < nThreads; ++i) {
					if (i < dimensions) {
						// positions
						dists[i].first.sample(sps[i], nParticles, (double*)(&tempPositions[i * nParticles]));
					}
					else {
						// velocities
						dists[i-dimensions].second.sample(sps[i], nParticles, (double*)(&tempVelocities[(i-dimensions) * nParticles]));
					}		
				}
			MC_CORE_TRACE("Generation of random numbers finished");
			// in memory, tempPositions now looks like [ ... xs  ... ys ... zs ... ]
			// and, similarly, tempVelocities looks like [ ... vxs ... vys ... vzs ... ]
			// i.e. both look like row-major matrices with contiguous storage
			// to organize the positions/velocities vectors by particle like [ x0, y0, z0, ... xN, yN, zN]
			// and [ vx0, vy0, vz0, ... vxN, vyN, vzN ], transpose the temporary target vectors into class member vectors
			MC_CORE_INFO("Initializing ensemble states");
			mkl_domatcopy('R', 'T', dimensions, particles, 1, tempPositions.data(), particles, (double*)positions.data(), dimensions);
			mkl_domatcopy('R', 'T', dimensions, particles, 1, tempVelocities.data(), particles, (double*)velocities.data(), dimensions);
			MC_CORE_INFO("Initialization finished");
		}
		catch (...) {
			MC_CORE_FATAL("Unable to create ensemble (probably insufficient memory) - exiting.");
			exit(-1);
		}
	}

}
