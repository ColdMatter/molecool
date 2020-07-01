#pragma once

#include <vector>
#include "Core.h"
#include "Random.h"

namespace molecool {

	enum class Particle { Rb, CaF, YbF};

	// a 3-component cartesian vector
	struct Vector {
		Vector(double x = 0, double y = 0, double z = 0)
			: x(x), y(y), z(z)
		{}
		double x, y, z;
	};

	// A State represents the classical phase-space coordinates of a particle
	// BEWARE - Don't add anything else unless you also rewrite the Ensemble class intitialization code
	// The only way that is currently so fast is because of the simplicity of this struct
	struct  State {

		// construct from position and velocity vectors
		State(const Vector& pos, const Vector& vel)
			: position(pos), velocity(vel)
		{}

		// construct from explicit position and velocity components
		State(double x = 0, double y = 0, double z = 0, double vx = 0, double vy = 0, double vz = 0)
			: position(x, y, z), velocity(vx, vy, vz)
		{}
		
		Vector position;
		Vector velocity;
	};

	class  Ensemble {
	
	public:

		Ensemble(int nParticles, Particle p, std::vector< std::pair< Distribution, Distribution> >& dists);
		inline int getNumberOfParticles() { return particles; }
		inline int getNumberOfDimensions() { return dimensions; }
		inline std::vector<double>& getPositions() { return positions; }
		inline std::vector<double>& getVelocities() { return velocities; }
	
	private:
		int particles;					// number of particles
		int dimensions;					// number of spatial dimensions
		Particle particle;
		std::vector<double> positions;	// organized by particle as [ x0, y0, z0, x1, y1, z1, ... ]
		std::vector<double> velocities;	// organized by particle as [ vx0, vy0, vz0, vx1, vy1, vz1, ... ]
		std::vector< std::pair< Distribution, Distribution> > distributions;

	};

}


