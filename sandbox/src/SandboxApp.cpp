/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

#include "molecool.h"
/*
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
*/



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
			dt = 0.5;

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
			/*
			auto trajTracker = [](const Ensemble & ens, double t) -> void {
				int n = 0;		// particle number 0..nParticles-1
				ParticleProxy p = ParticleProxy(ens, n);
				std::cout << "t = " << t << ", particle " << p.getIndex() << " at " << p.getPos() << std::endl;
			};
			addObserver(std::make_shared<trajTracker>);
			*/
			 
			addObserver(std::make_shared<Trajectorizer>(ensemble, 2));
			
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



