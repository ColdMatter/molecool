#pragma once

#include <vector>
#include "Core.h"
#include "Random.h"

namespace molecool {

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
		Ensemble(int nParticles, std::array<Distribution, 6>& dists);
		size_t getNumberOfParticles() { return states.size(); }
	
	private:
		std::vector<State> states;
		std::array<Distribution, 6> distributions;	// copy of distributions used to create the ensemble
	};

}


