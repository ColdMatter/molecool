#include "mcpch.h"
#include "Thruster.h"

namespace molecool {

	Thruster::Thruster(Ensemble& ens)
		: ensemble(ens)
	{
		auto lambda = [](ParticleProxy pp, double t) -> bool {
			return (pp.getX() > 0.5) && (t > 0.5);
		};
		registerFilter(lambda);
	}

	// this system function (odeint functor) has lots of side effects, this is probably unavoidable
	// the design of odeint guarantees that this system function is called by the main thread
	void Thruster::operator() (state_type const& x, state_type const& v, state_type& a, double t)
	{
		MC_PROFILE_FUNCTION();
		int nParticles = (int)x.size() / MC_DIMS;
		#pragma omp parallel for
		for (int i = 0; i < nParticles; ++i) {
			ParticleProxy p(ensemble, i);
			int j = MC_DIMS * i;					// particle index in x/v/a vectors
			if (i == 0) { printf("x: (%.6f, %.6f, %.6f), v: (%.6f, %.6f, %.6f), a: (%.6f, %.6f, %.6f)\n", x[j], x[j + 1], x[j + 2], v[j], v[j + 1], v[j + 2], a[j], a[j + 1], a[j + 2]); }
			if (!p.isActive()) 
			{	// particle is inactive, hold it at a fixed position (breaking the const promise by directly setting velocity)
				p.setVel(0, 0, 0);					// or double* vv = (double*)&v[j]; vv[0] = vv[1] = vv[2] = 0.0;
				a[j] = a[j + 1] = a[j + 2] = 0.0;
			}
			else if (filter(p, t))
			{	// 
				MC_CORE_TRACE("particle {0} lost at ({1},{2},{3})", i, p.getX(), p.getY(), p.getZ());
				p.deactivate();
				p.setVel(0, 0, 0);
				a[j] = a[j + 1] = a[j + 2] = 0.0;
			}
			else 
			{
				double m = ensemble.getParticleMass(i);	// better to include more information in State? like and extended Particle class?
				// Vector f = accumulateForces(st);
				for (int k = 0; k < MC_DIMS; ++k) {
					// a[j + k] = f[k] / m;
					a[j + k] = -x[j + k] - 0.1 * v[j + k];	// 'manual' damped harmonic oscillator
				}
			}
		} // end for all particles
	} // end function

	void Thruster::registerFilter(Filter fil) {
		filters.push_back(fil);
	}

	// apply all registerd filter functions to particle, returning true if any filter indicates particle should be filtered
	bool Thruster::filter(ParticleProxy pp, double t) {
		for (Filter f : filters) {
			if (f(pp, t)) { return true; }
		}
		return false;
	}

}
