/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

#include "molecool.h"

namespace molecool {

	class Sandbox : public Simulation {

	public:
		Sandbox() {

			Distribution xDist = Distribution(Shape::gaussian, 0.0, 1.0);
			Distribution vxDist = Distribution(Shape::gaussian, 0.0, 1.0);
			Distribution yDist = Distribution(Shape::gaussian, 0.0, 1.0);
			Distribution vyDist = Distribution(Shape::gaussian, 0.0, 1.0);
			Distribution zDist = Distribution(Shape::gaussian, 0.0, 1.0);
			Distribution vzDist = Distribution(Shape::gaussian, 0.0, 1.0);
			// create a vector to hold pairs of distributions and fill it
			std::vector< std::pair< Distribution, Distribution> > dists;
			dists.push_back(std::make_pair(xDist, vxDist));
			dists.push_back(std::make_pair(yDist, vyDist));
			dists.push_back(std::make_pair(zDist, vzDist));

			// generate the ensemble of particles and initialize them using the given distributions
			// 1e7 particles occupy ~500MB of heap memory (peak usage during construction is double that)
			int nParticles = 1'000'000;
			ensemble.addParticles(nParticles, ParticleId::CaF, dists);
		
		};

		~Sandbox() {};

	private:

	};


	// function called by the engine in the entry point
	// user/client simulation code executed before simulation runs goes here
	Simulation* createSimulation() {
		return new Sandbox();
	}

}



