#pragma once

#include <mkl.h>
#include <omp.h>
#include <array>

// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();


// PROGRAM EXECUTION STARTS HERE
int main(int argc, char** argv) {
	// engine initialization code goes here

	//-------------------------------------
	// logging initialiation and tests
	// TODO: move to initialization method
	molecool::Log::init();
	MC_CORE_INFO("WELCOME TO MOLECOOL ENGINE v{0}\n", molecool::getEngineVersion());
	MC_CORE_INFO("Engine initialization started");
	MC_CORE_INFO("Start logging test");
	int a = 10, b = 5;
	MC_CORE_TRACE("info message, a={0}, b={1}", a, b);
	MC_CORE_WARN("warn message");
	MC_CORE_ERROR("error message");
	MC_CORE_FATAL("critical message");
	// client logging example
	MC_INFO("a client logger info message");
	MC_CORE_INFO("End logging test\n");
	//-------------------------------------

	//-------------------------------------
	// quick test of MKL random number generation
	MC_CORE_INFO("Begin test of MKL random number generation");
	int seedy = (int)time(0);						// current time in seconds as a seed
	VSLStreamStatePtr stream;						// stream for random number generation
	vslNewStream(&stream, VSL_BRNG_MCG31, seedy); 	// stream, generator type, seed
	const int METHOD = 0;
	const int nValues = 100'000;
	std::array<double, nValues> randomArrayTarget;
	double mean = 0;
	double width = 1;
	{
		ScopedTimer timer("MKL RNG test");
		int status = vdRngGaussian(METHOD, stream, nValues, randomArrayTarget.data(), mean, width);
	}
	/*
	// printing the C++11 way using a range-based for loop
	// actually awkward to extract the current index in this case
	for (const auto& rand : randomArrayTarget) {
		MC_CORE_INFO("value {0} = {1}", &rand - &(randomArrayTarget[0]), rand);
		//printf("value %I64u = %f\n", &rand - &(randomArrayTarget[0]), rand);
	}
	*/
	MC_CORE_INFO("End test of MKL random number generation\n");
	//-------------------------------------
	
	
	//-------------------------------------
	MC_CORE_INFO("openmp test started");
	int maxThreads = omp_get_max_threads();
	MC_CORE_INFO("{0} (or less) threads are available", maxThreads);
	MC_CORE_INFO("{0} threads are configured for dynamic adjustment", omp_get_dynamic());
	//omp_set_dynamic(0);		// Explicitly disable dynamic teams
	omp_set_num_threads(maxThreads);		
	#pragma omp parallel
	{
		// This code will run on each thread (with num/id = 0..N-1)
		MC_CORE_TRACE("Running on thread {0}", omp_get_thread_num());
	}
	// We're out of the parallelized scope, now single processor
	MC_CORE_INFO("openmp test ended /n");
	//-------------------------------------
	

	MC_CORE_INFO("Engine initialization complete.");

	// run the user simulation
	auto sim = molecool::createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------

	return 0;
}
