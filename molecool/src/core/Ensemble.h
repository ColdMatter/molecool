#pragma once

#include <vector>
//#include <valarray>
#include "Core.h"
#include "Random.h"

namespace molecool {

	struct MOLECOOL_API Vector {
		Vector(double x = 0, double y = 0, double z = 0)
			: x(x), y(y), z(z)
		{}
		double x, y, z;
	};

	struct MOLECOOL_API Particle {

		// construct from position and velocity vectors
		Particle(const Vector& pos, const Vector& vel)
			: position(pos), velocity(vel)
		{}

		// construct from explicit position and velocity components
		Particle(double x = 0, double y = 0, double z = 0, double vx = 0, double vy = 0, double vz = 0)
			: position(x, y, z), velocity(vx, vy, vz)
		{}

		/*
		// compose from a valarray object of length 6
		Particle(const std::valarray<double>& vArray)
			: position(vArray[0], vArray[1], vArray[2]), velocity(vArray[3], vArray[4], vArray[5])
		{}
		*/

		Vector position;
		Vector velocity;
		// probably needs to have a reference to a particle manager or at least a (static) species id
		// though maybe that should be the job of the Ensemble class
		// position in the vector container gives a unique particle number
	};

	class MOLECOOL_API Ensemble {
	
	public:
		Ensemble(long nParticles, std::array<Distribution, 6>& dists);
		size_t getNumberOfParticles() { return particles.size(); }
	
	private:
		std::vector<Particle> particles;
		std::array<Distribution, 6> distributions;	// copy of distributions used to create the ensemble
	};

}


