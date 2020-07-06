#include "mcpch.h"
#include "Simulation.h"

namespace molecool {
    
    Simulation::Simulation() {
        MC_PROFILE_FUNCTION();
    }

    Simulation::~Simulation() {
        MC_PROFILE_FUNCTION();
    }
    
    void Simulation::run() {
        MC_PROFILE_FUNCTION();
        // this is where we should propagate the molecules
        // for now, just wait for input from user, any key press will close the program
        std::cout << "press any key to end" << std::endl;
        std::cin.get();
    }

}