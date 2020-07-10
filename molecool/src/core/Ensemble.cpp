#include "mcpch.h"
#include "Ensemble.h"
#include "Random.h"

namespace molecool {

	Ensemble::Ensemble() 
		: population(0)
	{
		MC_CORE_INFO("Creating ensemble");
	}

	void Ensemble::addParticles(size_t nParticles, ParticleId pId, std::array< std::pair< PosDist, VelDist>, MC_DIMS >& dists)
	{
		MC_PROFILE_FUNCTION();
		MC_CORE_INFO("Adding {0} particles of type {1}", nParticles, pId);
		try {
				size_t nDists = 2 * MC_DIMS;
				size_t nRandoms = nParticles * MC_DIMS;					
				std::vector<double> tempPositions(nRandoms);
				std::vector<double> tempVelocities(nRandoms);	
				
				{
					MC_PROFILE_SCOPE("ensemble memory allocation");
					positions.resize(positions.size() + nRandoms);
					velocities.resize(velocities.size() + nRandoms);
					particleIds.resize(particleIds.size() + nParticles);
				}
				
				{
					MC_PROFILE_SCOPE("ensemble random number generation");
					// For random number generation, make a thread for each phase-space coordinate for simplicity
					// create independent random number streams for each thread before parallel section
					int seed = (int)time(0);
					std::vector<std::shared_ptr<RandomStream>> sps;
					for (int i = 0; i < nDists; ++i) {
						sps.push_back(std::make_shared<RandomStream>(seed));
					}
					#pragma omp parallel for
					for (int i = 0; i < nDists; ++i) {
						if (i < MC_DIMS) {
							dists[i].first.sample(sps[i], nParticles, tempPositions, i * nParticles);
							// enforce first particle comes from distribution center(s):
							double* posPtr = (double*)&tempPositions[i * nParticles];
							posPtr[0] = dists[i].first.getPeak();	 
						}
						else {
							int in = i - MC_DIMS;
							dists[in].second.sample(sps[i], nParticles, tempVelocities, in * nParticles);
							// enforce first particle comes from distribution center(s):
							double* velPtr = (double*)&tempVelocities[in * nParticles];
							velPtr[0] = dists[in].second.getPeak();
						}
					}
				}
				// in memory, tempPositions now looks like [ ... xs  ... ys ... zs ... ]
				// and, similarly, tempVelocities looks like [ ... vxs ... vys ... vzs ... ]
				// i.e. both look like row-major matrices (with contiguous storage) with MC_DIMS rows and nParticles columns
				// to organize the vectors by particle like [ x0, y0, z0, ... xN, yN, zN] and [ vx0, vy0, vz0, ... vxN, vyN, vzN ],
				// transpose the temporary target vectors into class member vectors
				{
					MC_PROFILE_SCOPE("ensemble states initialization"); 
					double* posPtr = (double*)positions.data() + population * MC_DIMS;
					double* velPtr = (double*)velocities.data() + population * MC_DIMS;
					mkl_domatcopy('R', 'T', MC_DIMS, nParticles, 1, tempPositions.data(), nParticles, posPtr, MC_DIMS);
					mkl_domatcopy('R', 'T', MC_DIMS, nParticles, 1, tempVelocities.data(), nParticles, velPtr, MC_DIMS);
					int d = 2;
				}
		}
		catch (...) {
			MC_CORE_ERROR("Unable to add particles, exiting...");
			exit(-1);
		}

		// new particles have successfully been added to the ensemble
		for (int i = population; i < population + nParticles; ++i) {
			particleIds[i] = pId;
		}
		population += nParticles;
	}

}
