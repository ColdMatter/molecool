#include "mcpch.h"
#include "Thruster.h"

namespace molecool {

	Thruster::Thruster(Ensemble& ens)
		: ensemble(ens)
	{}

	Thruster::~Thruster() {
		MC_CORE_TRACE("Destroying thruster");
	}

	// this system function (odeint functor) has lots of side effects, this is probably unavoidable
	// the design of odeint guarantees that this system function is called by the main thread
	void Thruster::operator() (state_type const& x, state_type const& v, state_type& a, double t)
	{
		MC_PROFILE_FUNCTION();
		int nParticles = (int)x.size() / MC_DIMS;
		#pragma omp parallel for
		for (int i = 0; i < nParticles; ++i) {
			int j = MC_DIMS * i;									// particle index in x/v/a vectors
			Velocity& vel = (Velocity&)v[j];						// some refs to reduce error-prone manual indexing below
			Acceleration& acc = (Acceleration&)a[j];				// and improve readability
			const ParticleProxy& p = ParticleProxy(ensemble, i);
			if (!p.isActive()) 
			{	// particle not active, skip!
				continue; 
			}
			else if (filter(p, t))
			{	// check if an active particle should be filtered
				// filter actually evaluates as true 3 times before molecule is deactivated, allowing v,a to damp to zero before deactivation
				vel.x = vel.y = vel.z = 0;				// set velocity to zero, breaking the const promise
				if (acc.x == 0 && acc.y == 0.0 && acc.z == 0.0) 
				{	// acceleration has properly damped to zero, OK to never change it again 
					ensemble.deactivateParticle(i); 
					MC_CORE_TRACE("particle lost @ ({0}, {1}, {2})", p.getX(), p.getY(), p.getZ());
				}
				else 
				{	// particle matches filter condition but acceleration hasn't reached zero yet due to odeint internal state 
					acc.x = acc.y = acc.z = 0.0; 
				}
			}
			else 
			{	// normal propagation
				acc = getTotalForce(p, t) / p.getMass();
			}
			int d = 2;
		} // end for all particles
	} // end function

	void Thruster::addFilter(const FilterFunction& fil) {
		MC_CORE_TRACE("Adding filter");
		filters.push_back(fil);
	}

	inline bool Thruster::filter(const ParticleProxy& pp, double t) {
		for (auto& f : filters) {
			if (f(pp, t)) { return true; }
		}
		return false;
	}

	void Thruster::addForce(const ForceFunction& f) {
		MC_CORE_TRACE("Adding force");
		forces.push_back(f);
	}

	inline Force Thruster::getTotalForce(const ParticleProxy& pp, double t) {
		Force f;
		for (auto& ff : forces) {
			f += ff(pp, t);
		}
		return f;
	}

}
