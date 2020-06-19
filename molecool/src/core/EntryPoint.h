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
	// TODO: move to engine initialization method
	Log::init();
	MC_CORE_INFO("Welcome to the MOLECOOL engine v{0}", molecool::getEngineVersion());

	int maxThreads = omp_get_max_threads();
	omp_set_dynamic(0);
	MC_CORE_INFO("Hardware check: {0} cores/threads available", maxThreads, omp_get_dynamic());

	// test of using shared_ptr for RandomStream object lifetime management
	{
		double a[10];
		auto dist = Distribution(Shape::gaussian, 0, 1);
		auto sp = std::make_shared<RandomStream>();
		dist.sample(sp, 10, a);
		print1dArray(a);
	}	// sp and RandomStream objects are automatically deleted as last owner goes out of scope
	
	// test of parallelizing the generation of random numbers
	// each thread gets an independent stream but the same seed
	{	
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
			auto dist = Distribution(Shape::gaussian, 0, 1);
			dist.sample(sps[threadId], 10, tArray);
			MC_CORE_TRACE("message from thread {0}, first random number is {1}", threadId, tArray[0]);
		}
	}
	
	//-------------------------------------
	// A quick test of ensemble creation and initialization
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
	//-------------------------------------
	
	
	// run the user simulation
	auto sim = createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------

	return 0;
}
