#pragma once

// TO BE DEFINED BY CLIENT SIMULATION
extern molecool::Simulation* molecool::createSimulation();

int main(int argc, char** argv) {

	// engine initialization code goes here
	molecool::Log::init();
	molecool::Log::getCoreLogger()->info("Logger started");
	molecool::Log::getClientLogger()->warn("Logger started");
	//-------------------------------------

	// run the user simulation
	auto sim = molecool::createSimulation();
	sim->run();
	delete sim;
	//-------------------------------------
	return 0;
}