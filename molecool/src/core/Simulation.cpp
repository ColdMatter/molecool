#include "mcpch.h"
#include "Simulation.h"

namespace molecool {
    
    Simulation::Simulation() {
    }

    Simulation::~Simulation() {
    }
    
    void Simulation::run() {
        // for now, just wait for input from user, any key press will close the program
        std::cout << "press any key to end" << std::endl;
        std::cin.get();
    }

}