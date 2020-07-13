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
			int nParticles = 1'000'000;
			PosDist xDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vxDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist yDist = Dist(PDF::gaussian, 0.0, 1.0);
			VelDist vyDist = Dist(PDF::gaussian, 0.0, 1.0);
			PosDist zDist = Dist(PDF::delta, 0.0);
			VelDist vzDist = Dist(PDF::delta, 0.0);
			addParticles(nParticles, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);

			// then register watchers, forces, etc.

			// a particle filter
			auto filter = [](ParticleProxy pp, double t) -> bool {
				return (pp.getX() > 0.5) && (t > 0.5);
			};
			addFilter(filter);

			// add gravity
			auto gravity = [](ParticleProxy pp, double t) -> Force {
				double m = pp.getMass();
				const double g = 9.8;
				return Force(0, -m * g, 0);
			};
			addForce(gravity);

			// add damped simple harmonic oscillator force
			auto sho3d = [](ParticleProxy pp, double t) -> Force {
				double k = 1.0;			// spring constant
				double gam = 0.1;		// damping constant
				double fx = -k * pp.getX() - gam * pp.getVx();
				double fy = -k * pp.getY() - gam * pp.getVy();
				double fz = -k * pp.getZ() - gam * pp.getVz();
				return Force(fx, fy, fz);
			};
			addForce(sho3d);

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



