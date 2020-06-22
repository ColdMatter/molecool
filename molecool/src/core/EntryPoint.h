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
	Log::init();
	MC_CORE_INFO("Welcome to the MOLECOOL engine v{0}", molecool::getEngineVersion());

	omp_set_dynamic(0);
	MC_CORE_INFO("Hardware check: {0} cores/threads available", omp_get_max_threads(), omp_get_dynamic());
	
	// create and run the user simulation
	MC_INFO("Creating client simulation...");
	auto sim = createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------

	return 0;
}
