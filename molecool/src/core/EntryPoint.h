#pragma once
#include "Core.h"
#include "mcpch.h"
#include "lua.h"


// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();

using state_type = std::vector<double>;

class harm_osc {
	double m_gam;
	
public:
	harm_osc(double gam) : m_gam(gam) { }
	void operator() (state_type const& x, state_type const& v, state_type& a, double t)
	{
		MC_PROFILE_FUNCTION();
		int nDimensions = 3;
		int nOscillators = (int)x.size() / nDimensions;
		#pragma omp parallel for schedule(static)
		for (int i = 0; i < nOscillators; i++) {
			for (int j = 0; j < nDimensions; ++j) {
				a[i + j] = -x[i + j] - m_gam * v[i + j];
			}
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
	MC_CORE_INFO("Welcome to the MOLECOOL engine v{0}", getEngineVersion());

	omp_set_dynamic(0);
	MC_CORE_INFO("Hardware check: {0} cores/threads available", omp_get_max_threads(), omp_get_dynamic());

	// engine initialization code goes here
	MC_PROFILE_BEGIN_SESSION("startup");

	//-------------------------------------
	MC_INFO("Begin ODEINT ensemble propagation test");
	// A quick test of ensemble creation and initialization
	// construct desired position and velocity distributions
	Distribution xDist = Distribution(Shape::gaussian, 0, 1.0);
	Distribution vxDist = Distribution(Shape::gaussian, 0, 1.0);
	Distribution yDist = Distribution(Shape::gaussian, 0, 1.0);
	Distribution vyDist = Distribution(Shape::gaussian, 0, 1.0);
	Distribution zDist = Distribution(Shape::gaussian, 0, 1.0);
	Distribution vzDist = Distribution(Shape::gaussian, 0, 1.0);
	// create a vector to hold pairs of distributions and fill it
	std::vector< std::pair< Distribution, Distribution> > dists;
	dists.push_back(std::make_pair(xDist, vxDist));
	dists.push_back(std::make_pair(yDist, vyDist));
	dists.push_back(std::make_pair(zDist, vzDist));

	// generate the ensemble of particles and initialize them using the given distributions
	// 1e7 particles occupy ~500MB of heap memory (peak usage during construction is double that)
	long nParticles = 1'000'000;
	Ensemble ensemble(nParticles, Particle::CaF, dists);
	std::vector<double>& ps = ensemble.getPositions();		// get reference to positions vector to do stuff
	std::vector<double>& vs = ensemble.getVelocities();
	harm_osc ho(0.1);
	{
		MC_PROFILE_SCOPE("propagation");
		// using openmp algebra + standard operations + openmp system() function seems to reliably be the fastest
		// doesn't seem to be compatible with using the mkl operations, which appear to require a vector_algebra
		//using stepper_type = velocity_verlet< state_type >;
		//using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, vector_space_algebra, mkl_operations >;
		using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, openmp_range_algebra >;
		stepper_type stepper;
		const double startT = 0.0;
		const double dt = 0.001;
		const double endT = 1.0;
		integrate_const(stepper, ho, std::make_pair(std::ref(ps), std::ref(vs)), startT, endT, dt);
	}
	MC_INFO("End ODEINT ensemble propagation test");
	//-------------------------------------
	

	MC_INFO("Creating client simulation...");
	//MC_PROFILE_BEGIN_SESSION("startup");
	auto sim = createSimulation();
	MC_PROFILE_END_SESSION();

	MC_INFO("Running client simulation...");
	MC_PROFILE_BEGIN_SESSION("runtime");
	sim->run();
	MC_PROFILE_END_SESSION();

	MC_INFO("Ending client simulation...");
	MC_PROFILE_BEGIN_SESSION("shutdown");
	delete sim;
	MC_PROFILE_END_SESSION();
	//-------------------------------------

	return 0;
}
