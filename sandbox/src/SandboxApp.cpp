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
	Simulation* createSimulation() {
		//---------------------------------------------------------------------
		// user/client simulation code run before simulation proceeds goes here
		MC_INFO("Creating client simulation...");

		//---------------------------------------------------------------------
		return new Sandbox();
	}

}



