#pragma once

// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();


// PROGRAM EXECUTION STARTS HERE
int main(int argc, char** argv) {

	// engine initialization code goes here

	molecool::Log::init();
	// core logging examples
	MC_CORE_TRACE("trace message");
	int a = 10;
	MC_CORE_INFO("info message, a={0}", a);
	MC_CORE_WARN("warn message");
	MC_CORE_ERROR("error message");
	MC_CORE_FATAL("critical message");
	// client logging example
	MC_INFO("Logger started");
	//-------------------------------------

	// run the user simulation
	auto sim = molecool::createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------

	return EXIT_SUCCESS;
}