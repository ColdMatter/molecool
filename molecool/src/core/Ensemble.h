#pragma once

#include <vector>
#include "Core.h"
#include "Random.h"
#include "Vector.h"

namespace molecool {

	enum class ParticleId { Rb, CaF, YbF};


	class  Ensemble {
	
	public:

		Ensemble();
		void addParticles(int nParticles, ParticleId pId, std::array< std::pair< PosDist, VelDist>, MC_DIMS >& dists);
		inline size_t getPopulation() { return population; }
		inline std::vector<double>& getPos() { return pos; }
		inline std::vector<double>& getVel() { return vel; }
		
		// methods for manipulating or getting information about individual "particles"
		// prefer access using ParticleProxy instead
		bool isParticleActive(int i);
		double getParticleMass(int i);
		void deactivateParticle(int i);

		// request the number of particles that are still active
		int getActivePopulation();

		// ensemble (classical) state vectors organized by particle as [ x0, y0, z0, x1, y1, z1, ... ] for particle N = 0, 1, ... 
		std::vector<double> pos, vel;
	
	private:
		int population = 0;					// number of particles in the ensemble
		int active = 0;						// how many particles are active

		std::vector<ParticleId> particleIds;	// list of particle ids
		std::vector<bool> actives;				// vector of active flags for participating particles

	};

	// a lightweight 'Particle'-like object for accessing particles in the ensemble as if they were 
	// structured as an overall vector of Particle objects
	// this is useful for doing tests on individual elements, priting, etc.
	struct ParticleProxy {
		ParticleProxy(Ensemble& ensemble, int index)
			: n(index), ens(ensemble)
		{}

		int n;				// particle number/id 0..nParticles
		Ensemble& ens;

		int getIndex() { return n; }
		double getX() { return ens.pos[n * MC_DIMS]; }
		double getY() { return ens.pos[n * MC_DIMS + 1]; }
		double getZ() { return ens.pos[n * MC_DIMS + 2]; }
		double getVx() { return ens.vel[n * MC_DIMS]; }
		double getVy() { return ens.vel[n * MC_DIMS + 1]; }
		double getVz() { return ens.vel[n * MC_DIMS + 2]; }
		bool isActive() { return ens.isParticleActive(n); }
		void deactivate() { ens.deactivateParticle(n); }
		double getMass() { return ens.getParticleMass(n); }
		void setPos(double x, double y, double z) {
			int i = n * MC_DIMS;
			ens.pos[  i  ] = x;
			ens.pos[i + 1] = y;
			ens.pos[i + 2] = z;
		}
		void setVel(double vx, double vy, double vz) {
			int i = n * MC_DIMS;
			ens.vel[  i  ] = vx;
			ens.vel[i + 1] = vy;
			ens.vel[i + 2] = vz;
		}
	};

}


