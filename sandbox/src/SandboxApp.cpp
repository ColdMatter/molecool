/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

#include "molecool.h"

namespace molecool {

	// An example struct to illustrate how forces can be added 
	struct Gravity {
		// you can build a functor object where the force is returned when evaluating ()
		Force operator()(const ParticleProxy& pp, double t) { return Force(0, -pp.getMass() * 9.8, 0); }
		// or you can use a member function 
		Force memFunc(const ParticleProxy& pp, double t) { return Force(0, -pp.getMass() * 9.8, 0); }
		static Force staFunc(const ParticleProxy& pp, double t) { return Force(0, -pp.getMass() * 9.8, 0); }
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

			//////////////////////////////////////////////
			// create a particle ensemble to be simulated
			int nParticles = 100;
			PosDist xDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vxDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist yDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vyDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist zDist = Dist(PDF::delta, 0.0);
			VelDist vzDist = Dist(PDF::delta, 0.0);
			addParticles(nParticles, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);
			//////////////////////////////////////////////


			//////////////////////////////////////////////
			// register particle filter(s) to indicate when a particle should stop propagating
			auto filter = [](const ParticleProxy& pp, double t) -> bool {
				return (pp.getIndex() == 0) && (t > 0.5);
			};
			addFilter(filter);
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
			// register observer(s)
			auto trajTracker = [](const Ensemble& e, double t) -> void {
				int n = 0;		// particle number 0..nParticles-1
				ParticleProxy p = ParticleProxy(e, n);
				std::cout << "t = " << t << ", particle " << p.getIndex() << " at " << p.getPos() << std::endl;
			};
			addObserver(trajTracker);
			//////////////////////////////////////////////
			

			// that's it!
		
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



