/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

#include "molecool.h"

namespace molecool {

	class Sandbox : public Simulation {

	public:

		// client simulation class constructor does exactly that - it constructs the simulation
		Sandbox() {

			// add particles to the simulation according to the given initial distributions
			int nParticles = 1'000;
			PosDist xDist = Dist(PDF::gaussian, 0.0, 0.001);
			VelDist vxDist = Dist(PDF::gaussian, 1.0, 0.001);
			PosDist yDist = Dist(PDF::gaussian, 2.0, 0.001);
			VelDist vyDist = Dist(PDF::gaussian, 3.0, 0.001);
			PosDist zDist = Dist(PDF::delta, 4);
			VelDist vzDist = Dist(PDF::delta, 5);
			addParticles(nParticles, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);

			// then register watchers, forces, etc.

			// that's all there is to do!
		
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



