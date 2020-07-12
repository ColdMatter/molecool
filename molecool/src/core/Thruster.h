#pragma once

#include "mcpch.h"
#include "Ensemble.h"

namespace molecool {

	using state_type = std::vector<double>;
    using Filter = std::function<bool(ParticleProxy /*particle*/,double /*t*/)>;


    // a functor that knows how to calculate accelerations for particles in the simulation
    class Thruster
    {

    public:
        Thruster(Ensemble& ens);

		// odeint system function
		void operator() (state_type const& x, state_type const& v, state_type& a, double t);

        void registerFilter(Filter fil);

    private:

		Ensemble& ensemble;

        // a collection of filter functions that return true if a particle should be stopped
        std::vector<Filter> filters;

        // apply all filter tests
        bool filter(ParticleProxy pp, double t);

    };

}


