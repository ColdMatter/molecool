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
			int nParticles = 100;
			PosDist xDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vxDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist yDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vyDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist zDist = Dist(PDF::delta, 0.0);
			VelDist vzDist = Dist(PDF::delta, 0.0);
			addParticles(nParticles, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);

			// then register watchers, forces, etc.

			// a particle filter
			auto filter = [](const ParticleProxy& pp, double t) -> bool {
				return (pp.getIndex() == 0) && (t > 0.5);
			};
			addFilter(filter);
			
			
			// add gravity
			auto gravity = [](const ParticleProxy& pp, double t) -> Force {
				double m = pp.getMass();
				const double g = 9.8;
				return Force(0, -m * g, 0);
			};
			addForce(gravity);
			
			// add damped simple harmonic oscillator force
			auto sho3d = [](const ParticleProxy& pp, double t) -> Force {
				const double k = 1.0;		// spring constant
				const double gam = 0.1;		// damping constant
				return -k * pp.getPos() - gam * pp.getVel();
			};
			addForce(sho3d);
			
			// add an observer to track the trajectory of a particle (to the console)
			auto trajTracker = [](const Ensemble& e, double t) -> void {
				int n = 0;		// particle number 0..nParticles-1
				ParticleProxy p = ParticleProxy(e, n);
				printf("particle %d at (%.6f, %.6f, %.6f) at t=%.6f \n", n, p.getX(), p.getY(), p.getZ(), t);
			};
			addObserver(trajTracker);
			
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



