#pragma once

#include <stdio.h>

extern molecool::Simulation* molecool::createSimulation();

int main(int argc, char** argv) {
	printf("Welcome to the MOLECOOL simulation engine!");
	auto app = molecool::createSimulation();
	app->run();
	delete app;
}