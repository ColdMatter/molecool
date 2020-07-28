/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

#include "molecool.h"


namespace molecool {

	// An example struct to illustrate how forces can be added using different structures
	struct Gravity {
		Force operator()(const ParticleProxy& pp, double t) { return Force(0, -pp.getMass() * 9.8, 0); }		// functor
		Force memFunc(const ParticleProxy& pp, double t) { return Force(0, -pp.getMass() * 9.8, 0); }			// member function	
		static Force staFunc(const ParticleProxy& pp, double t) { return Force(0, -pp.getMass() * 9.8, 0); }	// static member function
	};

	// Alternatively, a free function can be used as a force callback
	// In this case, just a damping force opposing the current velocity
	Force damping(const ParticleProxy& pp, double t) {
		const double d = 0.1;		// damping coefficient
		return -d * pp.getVel();
	}


	class Sandbox : public Simulation {

	public:

		// client simulation class constructor does exactly that - it constructs the simulation
		Sandbox() {

			// set simulation time parameters
			tStart = 0.0;
			tEnd = 1.0;
			dt = 0.1;

			//////////////////////////////////////////////
			// create a particle ensemble to be simulated
			int nParticles = 10;
			PosDist xDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vxDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist yDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vyDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist zDist = Dist(PDF::delta, 0.0);
			VelDist vzDist = Dist(PDF::delta, 0.0);
			addParticles(nParticles, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);
			//////////////////////////////////////////////
			
			
			//////////////////////////////////////////////
			// register force(s)
			Gravity gravity;
			addForce(gravity);																					// use a functor
			//addForce(std::bind(&Gravity::memFunc, &gravity, std::placeholders::_1, std::placeholders::_2));	// use a member function
			//addForce(&Gravity::staFunc);																		// use a static member function
			addForce(damping);																					// use a free function
			auto sho3d = [](const ParticleProxy& pp, double t) -> Force {										// use a lambda
				const double k = 1.0;		// spring constant
				return -k * pp.getPos();
			};
			addForce(sho3d);
			//////////////////////////////////////////////


			//////////////////////////////////////////////
			// register particle filter(s) to indicate when a particle should stop propagating
			auto filter = [](const ParticleProxy& pp, double t) -> bool {
				return (pp.getIndex() == 0) && (t > 0.5);
			};
			addFilter(filter);
			//////////////////////////////////////////////
			
		
			//////////////////////////////////////////////
			// register observer(s)
			addObserver(std::make_shared<Trajectorizer>(2));
			//////////////////////////////////////////////
			

			// that's it!
		
		};

		~Sandbox() {};

	private:

	};

	class ScriptedSandbox : public Simulation {

	public:

		ScriptedSandbox() {

			// ensemble data comes from script!
			// at this point, the base class constructor has already been called
			// you can add additional built-in watchers/particles/forces etc. as you see fit
			// or register user classes in those categories that will be instantiated by the script
			// effectively mixing and matching class-based behavior with scripting behavior

			//////////////////////////////////////////////
			// register force(s)
			Gravity gravity;
			addForce(gravity);																					// use a functor
			//addForce(std::bind(&Gravity::memFunc, &gravity, std::placeholders::_1, std::placeholders::_2));	// use a member function
			//addForce(&Gravity::staFunc);																		// use a static member function
			addForce(damping);																					// use a free function
			auto sho3d = [](const ParticleProxy& pp, double t) -> Force {										// use a lambda
				const double k = 1.0;		// spring constant
				return -k * pp.getPos();
			};
			addForce(sho3d);
			//////////////////////////////////////////////


			//////////////////////////////////////////////
			// register particle filter(s) to indicate when a particle should stop propagating
			auto filter = [](const ParticleProxy& pp, double t) -> bool {
				return (pp.getIndex() == 0) && (t > 0.5);
			};
			addFilter(filter);
			//////////////////////////////////////////////

			// observers specified in script!
			//addObserver(std::make_shared<Staticizer>());

		}

		~ScriptedSandbox() {}
	};

	// function called by the engine in the entry point
	// user/client simulation code executed before simulation runs goes here
	Simulation* createSimulation() {
		return new ScriptedSandbox();
	}

}



