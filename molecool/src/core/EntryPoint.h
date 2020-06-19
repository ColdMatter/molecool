#pragma once

#include <mkl.h>
#include <omp.h>
#include <array>
#include <valarray>

#include "Ensemble.h"
#include "Random.h"

// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();

template <typename T, size_t rows, size_t cols>
void print2dArray(T(&array)[rows][cols])
{
	for (std::size_t i = 0; i < rows; ++i) {
		for (std::size_t j = 0; j < cols; ++j) {
			printf("%f ", array[i][j]);
		}
		printf("\n");
	}
}

template <typename T, size_t rows>
void print1dArray(T(&array)[rows])
{
	for (std::size_t i = 0; i < rows; ++i) {
		printf("%f ", array[i]);
	}
	printf("\n");
}

// a function reports the current reference count for a shared_ptr
// note: the count includes the one given to the function if pointer passed by value!
void fun(std::shared_ptr<molecool::RandomStream> sp)
{
	MC_CORE_TRACE("shared pointer reference count is {0}",sp.use_count());
}


// PROGRAM EXECUTION STARTS HERE
int main(int argc, char** argv) {

	using namespace molecool;

	// engine initialization code goes here

	//-------------------------------------
	// logging initialiation and tests
	// TODO: move to initialization method
	Log::init();
	MC_CORE_INFO("WELCOME TO MOLECOOL ENGINE v{0}\n", molecool::getEngineVersion());
	
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

	// test of using shared_ptr for MklRandomStream
	// probably want to share ownership of stream with distributions that will use it
	// by passing the (shared) stream to their constructor (as value, to increment the reference count)
	// so that they share ownership of that stream
	{
		double testArray[10];
		auto streamPtr = std::make_shared<RandomStream>();
		auto dist = Distribution(Shape::gaussian, 0, 1);
		fun(streamPtr);
		dist.sample(streamPtr->getStream(), 10, testArray);
		print1dArray(testArray);
	}	// streamPtr and RandomStream object should be automatically deleted as it goes out of scope

	{
		double a[10];
		auto sp = std::make_shared<RandomStream>();
		auto dist = ContinuousDistribution(sp, Shape::gaussian, 0, 1);
		fun(sp);
		dist.sample(10, a);
		print1dArray(a);
	}	// sp and RandomStream object are automatically deleted as last owner goes out of scope
	
	{	
		// test of parallelizing the generation of random numbers, each thread gets
		// each thread gets an independent stream but the same seed
		const int nThreads = 4;
		int seed = (int)time(0);
		std::vector<std::shared_ptr<RandomStream>> sps;
		for (int i = 0; i < nThreads; ++i) {
			sps.push_back(std::make_shared<RandomStream>(seed));
		}
		omp_set_num_threads(nThreads);
		#pragma omp parallel
		{
			int threadId = omp_get_thread_num();
			double tArray[10];
			auto dist = ContinuousDistribution(sps[threadId], Shape::gaussian, 0, 1);
			dist.sample(10, tArray);
			MC_CORE_TRACE("message from thread {0}, first random number is {1}", threadId, tArray[0]);
		}
	}
	

	//-------------------------------------
	// quick test of ensemble creation and initialization
	int seed = (int)time(0);						// current time in seconds as a seed
	VSLStreamStatePtr stream;						// stream state descriptor
	vslNewStream(&stream, VSL_BRNG_MT2203, seed); 	// stream, generator type, seed
	
	// construct desired molecule distributions for {x, y, z, vx, vy, vz}
	std::array<Distribution,6> distributions = {
		Distribution(Shape::gaussian, 0, 1),
		Distribution(Shape::flat, 0, 1),
		Distribution(Shape::exponential, 0, 1),
		Distribution(Shape::gaussian, 0, 1),
		Distribution(Shape::gaussian, 0, 1),
		Distribution(Shape::gaussian, 0, 1)
	};

	// generate the ensemble of particles and initialize them using the given distributions
	long nParticles = 1'000'000;	// 1e7 particles occupy about 500MB of heap memory
	Ensemble ensemble(nParticles, distributions);
	
	vslDeleteStream(&stream);
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
	auto sim = createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------

	return 0;
}
