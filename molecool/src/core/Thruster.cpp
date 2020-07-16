#include "mcpch.h"
#include "Thruster.h"

namespace molecool {

	Thruster::Thruster(Ensemble& ens)
		: ensemble(ens)
	{}

	// this system function (odeint functor) has lots of side effects, this is probably unavoidable
	// the design of odeint guarantees that this system function is called by the main thread
	void Thruster::operator() (state_type const& x, state_type const& v, state_type& a, double t)
	{
		MC_PROFILE_FUNCTION();
		int nParticles = (int)x.size() / MC_DIMS;
		Acceleration acc;
		#pragma omp parallel for
		for (int i = 0; i < nParticles; ++i) {
			int j = MC_DIMS * i;					// particle index in x/v/a vectors
			const ParticleProxy& p = ParticleProxy(ensemble, i);
			if (!p.isActive()) { continue; }
			else if (filter(p, t))
			{	// filter actually evaluates as true 3 times before molecule is deactivated, allowing v,a to damp to zero before deactivation
				// if you deactivate the particle on the first filter (=true) instance, you have to continually hold v=a=0 while inactive
				double* vv = (double*)&v[j]; vv[0] = vv[1] = vv[2] = 0.0;	// set velocity to zero, breaking the const promise
				if (a[j] == 0.0 && a[j + 1] == 0.0 && a[j + 2] == 0.0) 
				{	// acceleration has properly damped to zero, OK to never change it again 
					ensemble.deactivateParticle(i); 
					MC_CORE_TRACE("particle lost, x={0}, v={1}, a={2}", p.getPos(), p.getVel(), acc);
				}
				else 
				{	// particle matches filter condition but acceleration hasn't reached zero yet due to odeint internal state 
					a[j] = a[j + 1] = a[j + 2] = 0.0; 
				}
			}
			else 
			{
				acc = getTotalForce(p, t) / p.getMass();
				a[  j  ] = acc.x;
				a[j + 1] = acc.y;
				a[j + 2] = acc.z;
			}
		} // end for all particles
	} // end function

	void Thruster::addFilter(FilterFunction fil) {
		filters.push_back(fil);
	}

	// apply all registerd filter functions to particle, returning true if any filter indicates particle should be filtered
	inline bool Thruster::filter(const ParticleProxy& pp, double t) {
		for (FilterFunction f : filters) {
			if (f(pp, t)) { return true; }
		}
		return false;
	}

	void Thruster::addForce(ForceFunction f) {
		forces.push_back(f);
	}

	inline Force Thruster::getTotalForce(const ParticleProxy& pp, double t) {
		Force f;
		for (ForceFunction ff : forces) {
			f += ff(pp, t);
		}
		return f;
	}

}
