#pragma once

#include <vector>
#include "Core.h"
#include "Random.h"

namespace molecool {

	enum class ParticleId { Rb, CaF, YbF};


	// a particle (classical) state, used only to refer to elements of composite data structures in a reasonable way
	// just holds references to position and velocity components of the parent structures
	// let me re-iterate this, a State is only a collection of references for ease of testing/maniupulation
	// this isn't particularly good practice, if the vectors are resized then these references are dangling
	// but it should be OK for extraction and testing of particle positions/velocities during propagation
	// since at that point the ensemble vector containers won't be resized... I think
	struct State {

		double& xr; 
		double& yr;
		double& zr;
		double& vxr;
		double& vyr;
		double& vzr;

		// construct from a pointer to the x position and vx velocity
		State(double* p, double* v)
			: xr(p[0]), yr(p[1]), zr(p[2]), vxr(v[0]), vyr(v[1]), vzr(v[2])
		{}

		inline double& getX() { return xr; }
		inline double& getY() { return yr; }
		inline double& getZ() { return zr; }
		inline double& getVx() { return vxr; }
		inline double& getVy() { return vyr; }
		inline double& getVz() { return vzr; }

	};


	class  Ensemble {
	
	public:

		Ensemble();
		void addParticles(size_t nParticles, ParticleId pId, std::array< std::pair< PosDist, VelDist>, MC_DIMS >& dists);
		inline size_t getPopulation() { return population; }
		inline std::vector<double>& getPos() { return pos; }
		inline std::vector<double>& getVel() { return vel; }
		
		State getState(int i) { return State(&pos.at(i * MC_DIMS), &vel.at(i * MC_DIMS)); }
		
		// check if the ith particle is currently active
		bool isParticleActive(int i);

		double getParticleMass(int i);

		// deactivate a particle
		void deactivateParticle(int i);

		// request the number of particles that are still active
		int getActivePopulation();

		// ensemble (classical) state vectors organized by particle as [ x0, y0, z0, x1, y1, z1, ... ] for particle N = 0, 1, ... 
		std::vector<double> pos, vel;
	
	private:
		size_t population;						// number of particles in the ensemble

		std::vector<ParticleId> particleIds;	// list of particle ids
		std::vector<bool> actives;				// vector of active flags for participating particles
		size_t active = 0;						// how many molecules are active

	};

	// a lightweight 'Particle' object for accessing particles in the ensemble as if they were 
	// structured as an overall vector of Particle objects
	// this is useful for doing tests on individual elements, priting, etc.
	struct ParticleProxy {
		ParticleProxy(Ensemble& ensemble, int index)
			: in(index), ens(ensemble)
		{}

		int in;
		Ensemble& ens;

		double getX() { return ens.pos[in * MC_DIMS]; }
		double getY() { return ens.pos[in * MC_DIMS + 1]; }
		double getZ() { return ens.pos[in * MC_DIMS + 2]; }
		double getVx() { return ens.vel[in * MC_DIMS]; }
		double getVy() { return ens.vel[in * MC_DIMS + 1]; }
		bool isActive() { return ens.isParticleActive(in); }
		void deactivate() { ens.deactivateParticle(in); }
		double getMass() { return ens.getParticleMass(in); }
		void setPos(double x, double y, double z) {
			int i = in * MC_DIMS;
			ens.pos[  i  ] = x;
			ens.pos[i + 1] = y;
			ens.pos[i + 2] = z;
		}
		void setVel(double vx, double vy, double vz) {
			int i = in * MC_DIMS;
			ens.vel[  i  ] = vx;
			ens.vel[i + 1] = vy;
			ens.vel[i + 2] = vz;
		}
	};

}


