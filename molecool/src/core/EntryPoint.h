#pragma once
#include "Core.h"
#include "mcpch.h"
#include "lua.h"


// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();


// PROGRAM EXECUTION STARTS HERE
int main(int argc, char** argv) {

	using namespace molecool;

	//-------------------------------------
	// logging initialiation
	Log::init();
	MC_CORE_INFO("Welcome to the MOLECOOL engine v{0}", getEngineVersion());

	MC_CORE_INFO("Hardware check: {0} threads available", omp_get_max_threads());
	omp_set_dynamic(0);
	//omp_set_num_threads(3);	// uncomment or adjust to compare timing with different thread counts

	// engine initialization code goes here


	MC_INFO("Creating client simulation...");
	MC_PROFILE_BEGIN_SESSION("startup");
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
