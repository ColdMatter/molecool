#include "mcpch.h"
#include "Simulation.h"
#include "Timer.h"

namespace molecool {

    void print() {
        printf("link test");
    }
    
    Simulation::Simulation() {
    }

    Simulation::~Simulation() {
    }
    
    void Simulation::run() {
        molecool::TimerManager::getInstance()->summary();

        // for now, just wait for input from user, any key press will close the program
        std::cout << "press any key to end" << std::endl;
        std::cin.get();
    }

}