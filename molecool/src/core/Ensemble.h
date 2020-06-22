#pragma once

#include <vector>
#include <valarray>
#include "Core.h"
#include "Random.h"

namespace molecool {

	// a 3-component cartesian vector
	struct MOLECOOL_API Vector {
		Vector(double x = 0, double y = 0, double z = 0)
			: x(x), y(y), z(z)
		{}
		double x, y, z;
	};

	// A particle is a very simple object, it just constains the position and velocity of "something"
	// BEWARE - Don't add anything else unless you also rewrite the Ensemble class intitialization code
	// The only way that is currently so fast is because of how simple the Particle class is
	struct MOLECOOL_API Particle {

		// construct from position and velocity vectors
		Particle(const Vector& pos, const Vector& vel)
			: position(pos), velocity(vel)
		{}

		// construct from explicit position and velocity components
		Particle(double x = 0, double y = 0, double z = 0, double vx = 0, double vy = 0, double vz = 0)
			: position(x, y, z), velocity(vx, vy, vz)
		{}
		
		Vector position;
		Vector velocity;
	};

	class MOLECOOL_API Ensemble {
	
	public:
		Ensemble(int nParticles, std::array<Distribution, 6>& dists);
		size_t getNumberOfParticles() { return particles.size(); }
	
	private:
		std::vector<Particle> particles;
		std::array<Distribution, 6> distributions;	// copy of distributions used to create the ensemble
	};

}


