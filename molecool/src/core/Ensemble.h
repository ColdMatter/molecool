#pragma once

#include <vector>
#include <string>
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
		inline state_type& getPos() { return pos; }
		inline state_type& getVel() { return vel; }
		
		// methods for manipulating or getting information about individual "particles"
		// prefer access using ParticleProxy instead for readability
		inline bool isParticleActive(int i) const { return actives.at(i); }
		inline double getParticleMass(int i) const { return 1; }
		void deactivateParticle(int i);
		inline Position getParticlePos(int i) const { return Position( (double*)&pos[i * MC_DIMS] ); }
		inline Velocity getParticleVel(int i) const { return Velocity( (double*)&vel[i * MC_DIMS] ); }

		void save(std::string filename);

		// ensemble (classical) state vectors organized by particle as [ x0, y0, z0, x1, y1, z1, ... ] for particle N = 0, 1, ... 
		state_type pos, vel;
	
	private:
		int population = 0;					// number of active particles in the ensemble

		std::vector<ParticleId> particleIds;	// list of particle ids
		std::vector<bool> actives;				// vector of active flags for participating particles

	};

	// a lightweight 'Particle'-like object for accessing particles in the ensemble as if they were 
	// structured as an overall vector of Particle objects
	// this is useful for doing tests on individual elements, priting, etc.
	struct ParticleProxy {
		ParticleProxy(const Ensemble& ensemble, int index)
			: n(index), i(n * MC_DIMS), ens(ensemble)
		{}

		const int n;				// particle number/id 0..nParticles
		const int i;				// index in various vectors
		const Ensemble& ens;

		int getIndex() const { return n; }
		const double& getX() const { return ens.pos[  i  ]; }
		const double& getY() const { return ens.pos[i + 1]; }
		const double& getZ() const { return ens.pos[i + 2]; }
		const double& getVx() const { return ens.vel[  i  ]; }
		const double& getVy() const { return ens.vel[i + 1]; }
		const double& getVz() const { return ens.vel[i + 2]; }
		const Position getPos() const { return ens.getParticlePos(n); }
		const Velocity getVel() const { return ens.getParticleVel(n); }
		const bool isActive() const { return ens.isParticleActive(n); }
		const double getMass() const { return ens.getParticleMass(n); }
	};

}


