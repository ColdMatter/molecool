/*
This is a prototype simulation that uses the 'molecool' simulation engine.
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
	Simulation* createSimulation() {
		MC_PROFILE_FUNCTION();

		// here is where we should initialize the ensemble, setup the environment, etc. based on the 
		// external json file 'simulation.json', which should probably come in as an argument to main()


		//---------------------------------------------------------------------
		return new Sandbox();
	}

}



