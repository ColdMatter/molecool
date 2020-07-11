#pragma once

#include "mcpch.h"
#include "Ensemble.h"

namespace molecool {

	using state_type = std::vector<double>;


    // a functor that knows how to calculate accelerations for particles in the simulation
    class Thruster
    {

    public:
        Thruster(Ensemble& ens);

		// odeint system function
		void operator() (state_type const& x, state_type const& v, state_type& a, double t);


    private:

		Ensemble& ensemble;

    };

}


