/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

/* to link to the engine:
(1) add molecool/molecule/src to c++ include directories, include "molecool.h"
(2) add a reference to the 'molecool' project [project properties -> add reference]
(3) Add dll path to linker directories [in project properties -> linker options]
*/
#include "molecool.h"

namespace molecool {

	class Sandbox : public Simulation {

	public:
		Sandbox() {};
		~Sandbox() {};
	};


	// function called by the engine in the entry point
	// user/client simulation code executed before simulation runs goes here
	// Basically, DEFINE YOUR SIMULATION HERE
	Simulation* createSimulation() {

		//-------------------------------------
		// A quick test of ensemble creation and initialization
		// construct desired molecule distributions
		std::array<Distribution, 6> distributions = {
			Distribution(Shape::gaussian, 0, 0.1),	// x
			Distribution(Shape::gaussian, 1, 0.1),	// y
			Distribution(Shape::gaussian, 2, 0.1),	// z
			Distribution(Shape::gaussian, 3, 0.1),	// vx
			Distribution(Shape::gaussian, 4, 0.1),	// vy
			Distribution(Shape::gaussian, 5, 0.1)	// vz
		};
		// generate the ensemble of particles and initialize them using the given distributions
		// 1e7 particles occupy ~500MB of heap memory (peak usage during construction is double that)
		long nParticles = 10'000'000;	
		Ensemble ensemble(nParticles, distributions);
		//-------------------------------------



		//---------------------------------------------------------------------
		return new Sandbox();
	}

}



