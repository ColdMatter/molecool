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
		// construct desired position and velocity distributions
		Distribution xDist = Distribution(Shape::gaussian, 0, 0.1);
		Distribution vxDist = Distribution(Shape::gaussian, 1, 0.1);
		Distribution yDist = Distribution(Shape::gaussian, 2, 0.1);
		Distribution vyDist = Distribution(Shape::gaussian, 3, 0.1);
		Distribution zDist = Distribution(Shape::gaussian, 4, 0.1);
		Distribution vzDist = Distribution(Shape::gaussian, 5, 0.1);
		// create a vector to hold pairs of distributions and fill it
		std::vector< std::pair< Distribution, Distribution> > dists;
		dists.push_back(std::make_pair(xDist, vxDist));
		dists.push_back(std::make_pair(yDist, vyDist));
		dists.push_back(std::make_pair(zDist, vzDist));

		// generate the ensemble of particles and initialize them using the given distributions
		// 1e7 particles occupy ~500MB of heap memory (peak usage during construction is double that)
		long nParticles = 1'000'000;	
		Ensemble ensemble(nParticles, Particle::CaF, dists);

		std::vector<double>& ps = ensemble.getPositions();		// get reference to positions vector to do stuff
		std::vector<double>& vs = ensemble.getVelocities();
		ps[0] = 10.0;											// like direct access or assignment

		//-------------------------------------



		//---------------------------------------------------------------------
		return new Sandbox();
	}

}



