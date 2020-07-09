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
		int nOscillators = (int)x.size() / MC_DIMS;
		#pragma omp parallel for
		for (int i = 0; i < nOscillators; i+=MC_DIMS) {
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


// PROGRAM EXECUTION STARTS HERE
int main(int argc, char** argv) {

	using namespace molecool;
	using namespace boost::numeric::odeint;

	//-------------------------------------
	// logging initialiation
	Log::init();
	MC_CORE_INFO("Welcome to the MOLECOOL engine v{0}", getEngineVersion());

	MC_CORE_INFO("Hardware check: {0} threads available", omp_get_max_threads());
	omp_set_dynamic(0);
	//omp_set_num_threads(3);	// uncomment or adjust to compare timing with different thread counts

	// engine initialization code goes here
	MC_PROFILE_BEGIN_SESSION("startup");

	//-------------------------------------
	MC_INFO("Begin ODEINT ensemble propagation test");
	// A quick test of ensemble creation and initialization
	// construct desired position and velocity distributions
	Distribution xDist = Distribution(Shape::gaussian, 0.0, 1.0);
	Distribution vxDist = Distribution(Shape::gaussian, 0.0, 1.0);
	Distribution yDist = Distribution(Shape::gaussian, 0.0, 1.0);
	Distribution vyDist = Distribution(Shape::gaussian, 0.0, 1.0);
	Distribution zDist = Distribution(Shape::gaussian, 0.0, 1.0);
	Distribution vzDist = Distribution(Shape::gaussian, 0.0, 1.0);
	// create a vector to hold pairs of distributions and fill it
	std::vector< std::pair< Distribution, Distribution> > dists;
	dists.push_back(std::make_pair(xDist, vxDist));
	dists.push_back(std::make_pair(yDist, vyDist));
	//dists.push_back(std::make_pair(zDist, vzDist));

	// generate the ensemble of particles and initialize them using the given distributions
	// 1e7 particles occupy ~500MB of heap memory (peak usage during construction is double that)
	int nParticles = 1000;
	Ensemble ensemble;
	ensemble.addParticles(nParticles, ParticleId::CaF, dists);
	harm_osc ho(0.1);
	{
		MC_PROFILE_SCOPE("propagation");
		// using openmp algebra + standard operations + openmp system() function seems to reliably be the fastest
		// doesn't seem to be compatible with using the mkl operations, which require a vector_space_algebra
		// I think the absolute fastest would be to manually parallelize and have each thread use independent 
		// (non parallelized) steppers, system functions, and mkl operations
		//using stepper_type = velocity_verlet< state_type >;
		//using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, vector_space_algebra, mkl_operations >;
		using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, openmp_range_algebra >;
		stepper_type stepper;
		const double startT = 0.0;
		const double dt = 0.001;
		const double endT = 1.0;
		MC_INFO("propagating...");

		// integrate/propagate
		//integrate_const(stepper, ho, std::make_pair(std::ref(ps), std::ref(vs)), startT, endT, dt);
		
		// taking discrete steps is better, we can break out early if no particles are active
		for (double t = startT; t < endT; t += dt) {
			stepper.do_step(ho, std::make_pair(std::ref(ensemble.getPositions()), std::ref(ensemble.getVelocities())), t, dt);
			// independent steps allow us to test for boundary conditions, lost molecules [-> set a=0,v=0], etc.
			// but it would be more efficient to do these checks in the system function!
			// that can be done (or in the observer is more natural), just requires breaking the const promise!
			// HERE WE SHOULD TEST IF WE SHOULD BREAK OUT OF THE LOOP
			// if (nActiveParticles == 0) { break; }
		}
			
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
