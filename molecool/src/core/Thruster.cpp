#include "mcpch.h"
#include "Thruster.h"

namespace molecool {

	Thruster::Thruster(Ensemble& ens)
		: ensemble(ens)
	{}

	// the system function (functor) has lots of side effects, this is probably unavoidable
	void Thruster::operator() (state_type const& x, state_type const& v, state_type& a, double t)
	{
		MC_PROFILE_FUNCTION();
		int nParticles = (int)x.size() / MC_DIMS;
		#pragma omp parallel for
		for (int i = 0; i < nParticles; ++i) {
			ParticleProxy p(ensemble, i);
			int j = MC_DIMS * i;						// particle index in vectors
			if (i == 0) { printf("x: (%.6f, %.6f, %.6f), v: (%.6f, %.6f, %.6f), a: (%.6f, %.6f, %.6f)\n", x[j], x[j + 1], x[j + 2], v[j], v[j + 1], v[j + 2], a[j], a[j + 1], a[j + 2]); }
			if (!p.isActive()) { 
				// particle is inactive, hold it at a fixed position (breaking the const promise by directly setting velocity components)
				a[j] = a[j + 1] = a[j + 2] = 0.0;
				p.setVel(0, 0, 0);		// or double* vv = (double*)&v[j]; vv[0] = vv[1] = vv[2] = 0.0;
			}
			else if (i == 0 && t > 0.5) // applyFilterTests( Particle(ensemble, i) );	// extract particle for testing barrier conditions etc.)
			{
				MC_CORE_TRACE("particle {0} lost at ({1},{2},{3})", i, p.getX(), p.getY(), p.getZ());
				p.deactivate();
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


}
