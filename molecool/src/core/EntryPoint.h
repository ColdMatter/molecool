#pragma once

#include "mcpch.h"
#include "Ensemble.h"
#include "Random.h"

#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/openmp/openmp.hpp>

// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();


using state_type = std::vector<double>;
//typedef std::vector<double> state_type;


class harm_osc {
	double m_gam;
	
public:
	harm_osc(double gam) : m_gam(gam) { }
	void operator() (state_type const& x, state_type const& v, state_type& a, double t)
	{
		int nDimensions = 1;
		int nOscillators = x.size(); // / nDimensions;
		for (size_t i = 0; i < nOscillators; i+=nDimensions) {
			a[i] = -x[i] - m_gam * v[i];	// x dimension acceleration
			// a[i+1] = ;					// y acceleration
			// a[i+2] = ;					// z acceleration
		}
	}
};


// PROGRAM EXECUTION STARTS HERE
int main(int argc, char** argv) {

	using namespace molecool;
	using namespace boost::numeric::odeint;

	//-------------------------------------
	// logging initialiation
	Log::init();
	MC_CORE_INFO("Welcome to the MOLECOOL engine v{0}", molecool::getEngineVersion());

	omp_set_dynamic(0);
	MC_CORE_INFO("Hardware check: {0} cores/threads available", omp_get_max_threads(), omp_get_dynamic());

	// engine initialization code goes here

	MC_INFO("Begin ODEINT harmonic oscillator test");
	const int nOscillators = 100000;
	state_type x(nOscillators);
	state_type v(nOscillators);
	// initial conditions
	for (size_t i = 0; i < nOscillators; ++i) {
		x[i] = 1.0 + .1 * i;
		v[i] = 0.1*i;
		//MC_TRACE("particle {0} started at {1} w/ vel {2}", i, x[i], v[i]);
	}
	// standard algebra + operations
	using stepper_type = velocity_verlet< state_type >;	
	// standard algebra + mkl operations
	//using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, range_algebra, mkl_operations >;
	// openmp algebra + mkl operations
	//using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, openmp_range_algebra, mkl_operations >;
	stepper_type stepper;
	const double t0 = 0.0;
	const double dt = 0.1;
	double endT = 10.0;
	harm_osc ho(0.15);
	/*
	for (double t = t0; t < endT; t += dt) {
		stepper.do_step(ho2, std::make_pair(std::ref(x), std::ref(v)), t, dt);
	}
	*/
	MC_INFO("--- integration started ---");
	integrate_const(stepper, ho, std::make_pair(std::ref(x), std::ref(v)), t0, endT, dt);
	MC_INFO("--- integration finished ---");
	for (int i = 0; i < x.size(); ++i) {
		//MC_TRACE("particle {0} ended at {1} w/ vel {2}", i, x[i], v[i]);
	}

	MC_INFO("End ODEINT harmonic oscillator test");
	

	// create and run the user simulation
	MC_INFO("Creating client simulation...");
	auto sim = createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------

	return 0;
}
