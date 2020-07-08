#include "mcpch.h"
#include "Ensemble.h"
#include "Random.h"

namespace molecool {

	Ensemble::Ensemble(int nParticles, Particle p, std::vector< std::pair< Distribution, Distribution> >& dists)
		: particles(nParticles), particle(p), distributions(dists)
	{
		MC_PROFILE_FUNCTION();
		MC_CORE_INFO("Creating an ensemble of {0} particles of type {1}", particles, particle);
		try {
				dimensions = (int)distributions.size();
				MC_CORE_INFO("Dimensionality is {0}", dimensions);
				size_t nRandoms = (size_t)(particles * dimensions);
				assert(dimensions > 0);
				assert(particles > 0);

				std::vector<double> tempPositions;
				std::vector<double> tempVelocities;	
				
				{
					MC_PROFILE_SCOPE("ensemble memory allocation");
					positions.resize(nRandoms);
					velocities.resize(nRandoms);
					tempPositions.resize(nRandoms);
					tempVelocities.resize(nRandoms);
				}
				
				{
					MC_PROFILE_SCOPE("ensemble random number generation");
					// For random number generation, make a thread for each phase-space coordinate for simplicity
					// create independent random number streams for each thread before parallel section
					int seed = (int)time(0);
					int nDists = 2 * dimensions;
					std::vector<std::shared_ptr<RandomStream>> sps;
					for (int i = 0; i < nDists; ++i) {
						sps.push_back(std::make_shared<RandomStream>(seed));
					}
					#pragma omp parallel for
					for (int i = 0; i < nDists; ++i) {
						if (i < dimensions) {
							// positions
							dists[i].first.sample(sps[i], nParticles, (double*)(&tempPositions[i * nParticles]));
						}
						else {
							// velocities
							dists[i - dimensions].second.sample(sps[i], nParticles, (double*)(&tempVelocities[(i - dimensions) * nParticles]));
						}
					}
				}
				// in memory, tempPositions now looks like [ ... xs  ... ys ... zs ... ]
				// and, similarly, tempVelocities looks like [ ... vxs ... vys ... vzs ... ]
				// i.e. both look like row-major matrices with contiguous storage
				// to organize the vectors by particle like [ x0, y0, z0, ... xN, yN, zN] and [ vx0, vy0, vz0, ... vxN, vyN, vzN ],
				// transpose the temporary target vectors into class member vectors
				{
					MC_PROFILE_SCOPE("ensemble states initialization");
					mkl_domatcopy('R', 'T', dimensions, particles, 1, tempPositions.data(), particles, (double*)positions.data(), dimensions);
					mkl_domatcopy('R', 'T', dimensions, particles, 1, tempVelocities.data(), particles, (double*)velocities.data(), dimensions);
				}
		}
		catch (...) {
			MC_CORE_FATAL("Unable to create ensemble - exiting.");
			exit(-1);
		}
	}

}
