#include "mcpch.h"
#include "Simulation.h"

namespace molecool {
    
    Simulation::Simulation() 
    : thruster(ensemble)
    {
        MC_PROFILE_FUNCTION();
    }

    Simulation::~Simulation() {
        MC_PROFILE_FUNCTION();
    }
    
    void Simulation::run() {
        MC_PROFILE_FUNCTION();
        // ensemble.save("initials.txt");
        thruster.propagate();
        // ensemble.save("finals.txt");
        std::cout << "press any key to end" << std::endl;
        std::cin.get();
    }

}