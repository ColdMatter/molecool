#include "mcpch.h"
#include "Ensemble.h"
#include "Random.h"

namespace molecool {

	Ensemble::Ensemble() 
		: population(0)
	{
		MC_CORE_TRACE("Creating ensemble");
	}

	void Ensemble::addParticles(int nParticles, ParticleId pId, std::array< std::pair< PosDist, VelDist>, MC_DIMS >& dists)
	{
		MC_PROFILE_FUNCTION();
		MC_CORE_TRACE("Adding {0} particles of type {1}", nParticles, pId);
		try {
				size_t nDists = 2 * MC_DIMS;
				size_t nRandoms = nParticles * MC_DIMS;	
				std::vector<double> tempPos(nRandoms);
				std::vector<double> tempVel(nRandoms);
				
				{
					MC_PROFILE_SCOPE("ensemble memory allocation");
					pos.resize(pos.size() + nRandoms);
					vel.resize(pos.size());

					particleIds.resize(particleIds.size() + nParticles);
					actives.resize(actives.size() + nParticles);
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
							dists[i].first.sample(sps[i], nParticles, tempPos, i * nParticles);
							// enforce first particle comes from distribution center(s):
							double* posPtr = (double*)&tempPos[i * nParticles];
							posPtr[0] = dists[i].first.getPeak();	 
						}
						else {
							int in = i - MC_DIMS;
							dists[in].second.sample(sps[i], nParticles, tempVel, in * nParticles);
							// enforce first particle comes from distribution center(s):
							double* velPtr = (double*)&tempVel[in * nParticles];
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
					double* posPtr = (double*)pos.data() + population * MC_DIMS;
					double* velPtr = (double*)vel.data() + population * MC_DIMS;
					mkl_domatcopy('R', 'T', MC_DIMS, nParticles, 1, tempPos.data(), nParticles, posPtr, MC_DIMS);
					mkl_domatcopy('R', 'T', MC_DIMS, nParticles, 1, tempVel.data(), nParticles, velPtr, MC_DIMS);
				}
		}
		catch (...) {
			MC_CORE_FATAL("Unable to add particles, exiting...");
			exit(-1);
		}

		// new particles have successfully been added to the ensemble, record their ids and make them active
		for (int i = population; i < population + nParticles; ++i) {
			particleIds[i] = pId;
			actives[i] = true;
		}
		population += nParticles;
	}

	void Ensemble::deactivateParticle(int i) {
		actives.at(i) = false;
		population--;
	}

	void Ensemble::save(std::string filename) {
		MC_PROFILE_FUNCTION();
		MC_CORE_TRACE("saving ensemble states");
		std::ofstream outputStream;
		outputStream.open("output/" + filename + ".json");
		if (!outputStream.is_open())
		{
			MC_CORE_ERROR("ensemble could not open output file");
			return;
		}
		outputStream << std::fixed << std::setprecision(6);
		outputStream << "{\"" + filename + "\":[";
		for (int i = 0; i < population; ++i) {
			if (i > 0) { outputStream << ","; }
			outputStream << "{\"x\":[" << getParticlePos(i) << "],";
			outputStream << "\"v\":[" << getParticleVel(i) << "]}";
		}
		outputStream << "]}";
		outputStream.flush();
		outputStream.close();
	}

}
