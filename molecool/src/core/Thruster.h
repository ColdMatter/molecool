#pragma once

#include "mcpch.h"
#include "Ensemble.h"
#include "Vector.h"

namespace molecool {

    using FilterFunction = std::function< bool(ParticleProxy /*particle*/, double /*t*/) >;
    using ForceFunction = std::function< Force(ParticleProxy /*particle*/, double /*t*/) >;


    // a functor that knows how to calculate accelerations for particles in the simulation
    class Thruster
    {

    public:
        Thruster(Ensemble& ens);

		// odeint system function, signature is specific to 2nd order system for velocity-verlet stepper
		void operator() (state_type const& x, state_type const& v, state_type& a, double t);

        void addFilter(FilterFunction f);
        void addForce(ForceFunction f);

    private:

		Ensemble& ensemble;

        // a collection of filter functions that return true if a particle should be stopped
        std::vector<FilterFunction> filters;

        // a collection of force functions that apply forces based on position, velocity, etc.
        std::vector<ForceFunction> forces;

        // apply all filter tests
        inline bool filter(ParticleProxy pp, double t);

        // get sum of all acting forces
        inline Force getTotalForce(ParticleProxy pp, double t);

    };

}


