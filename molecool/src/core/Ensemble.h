#pragma once

#include <vector>
#include "Core.h"
#include "Random.h"

namespace molecool {

	enum class ParticleId { Rb, CaF, YbF};

	// a 3-component cartesian vector
	struct Vector {
		Vector(double x = 0, double y = 0, double z = 0)
			: x(x), y(y), z(z)
		{}
		double x, y, z;
	};

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
		Ensemble(size_t nParticles, ParticleId pId, std::vector< std::pair< Distribution, Distribution> >& dists);
		void addParticles(size_t nParticles, ParticleId pId, std::vector< std::pair< Distribution, Distribution> >& dists);
		inline size_t getPopulation() { return population; }
		inline std::vector<double>& getPositions() { return positions; }
		inline std::vector<double>& getVelocities() { return velocities; }
		inline State getState(int i) { return State(&positions.at(i * MC_DIMS), &velocities.at(i * MC_DIMS)); }
	
	private:
		size_t population;						// number of particles in the ensemble
		std::vector<double> positions;			// organized by particle as [ x0, y0, z0, x1, y1, z1, ... ]
		std::vector<double> velocities;			// organized by particle as [ vx0, vy0, vz0, vx1, vy1, vz1, ... ]
		std::vector<ParticleId> particleIds;	// list of particle ids

	};

}


