#pragma once

#include "mcpch.h"
#include "Ensemble.h"

namespace molecool {

	using state_type = std::vector<double>;

	class harm_osc {
		double m_gam;

	public:
		
		harm_osc(double gam) : m_gam(gam) { }
		
		void operator() (state_type const& x, state_type const& v, state_type& a, double t)
		{
			MC_PROFILE_FUNCTION();
			int nOscillators = (int)x.size() / MC_DIMS;
			#pragma omp parallel for
			for (int i = 0; i < nOscillators; i += MC_DIMS) {
				// if (!particleIsActive) { continue; }					// acceleration unchanged unless particle is currently active
				molecool::State st((double*)&x[i], (double*)&v[i]);		// extract state for testing barrier conditions etc.
				//bool filterParticle = filterTests(st);				// test vector of filters to see if particle should be stopped
				for (int j = 0; j < MC_DIMS; ++j) {
					if (false) { // (filterParticle) {
						MC_CORE_TRACE("particle {0} lost at ({1},{2},{3})", i, x[i], x[i + 1], x[i + 2]);
						// this is naughty, breaking the const promise for efficiency purposes (to avoid scanning through after timestep)
						double* vv = (double*)&v[i];			// break the const promise!
						*vv = 0.0;								// set velocity to zero
						a[i + j] = 0.0;							// and acceleration
					}
					else {
						// accumulate accelerations here
						a[i + j] = -x[i + j] - m_gam * v[i + j];
					}

				}
			}
		}
	};

    // a functor that knows how to calculate accelerations for particles in the simulation
    class Thruster
    {

    public:
        Thruster(Ensemble& ens);

		// odeint system function
		void operator() (state_type const& x, state_type const& v, state_type& a, double t);

    private:

		Ensemble& ensemble;
		double m_gam;
    };

}


