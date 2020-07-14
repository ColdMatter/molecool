#pragma once

#include <vector>
#include "Core.h"
#include "Random.h"
#include "Vector.h"

namespace molecool {

	// state type for odeint propagation
	using state_type = std::vector<double>;

	enum class ParticleId { Rb, CaF, YbF};


	class  Ensemble {
	
	public:

		Ensemble();
		void addParticles(int nParticles, ParticleId pId, std::array< std::pair< PosDist, VelDist>, MC_DIMS >& dists);
		inline int getPopulation() const { return population; }
		inline int getActivePopulation() const { return active; }
		inline state_type& getPos() { return pos; }
		inline state_type& getVel() { return vel; }
		
		// methods for manipulating or getting information about individual "particles"
		// prefer access using ParticleProxy instead for readability
		inline bool isParticleActive(int i) const { return actives.at(i); }
		inline double getParticleMass(int i) const { return 1; }
		void deactivateParticle(int i);

		// ensemble (classical) state vectors organized by particle as [ x0, y0, z0, x1, y1, z1, ... ] for particle N = 0, 1, ... 
		state_type pos, vel;
	
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
			: n(index), i(n * MC_DIMS), ens(ensemble)
		{}

		const int n;				// particle number/id 0..nParticles
		const int i;				// index in various vectors
		Ensemble& ens;

		int getIndex() const { return n; }
		double& getX() const { return ens.pos[  i  ]; }
		double& getY() const { return ens.pos[i + 1]; }
		double& getZ() const { return ens.pos[i + 2]; }
		double& getVx() const { return ens.vel[  i  ]; }
		double& getVy() const { return ens.vel[i + 1]; }
		double& getVz() const { return ens.vel[i + 2]; }
		Position getPos() const { return Position( getX(), getY(), getZ() ); }
		Velocity getVel() const { return Position( getVx(), getVy(), getVz() ); }
		bool isActive() const { return ens.isParticleActive(n); }
		void deactivate() const { ens.deactivateParticle(n); }
		double getMass() const { return ens.getParticleMass(n); }
		void setPos(double x, double y, double z) const {
			ens.pos[  i  ] = x;
			ens.pos[i + 1] = y;
			ens.pos[i + 2] = z;
		}
		void setVel(double vx, double vy, double vz) const {
			ens.vel[  i  ] = vx;
			ens.vel[i + 1] = vy;
			ens.vel[i + 2] = vz;
		}
	};

}


