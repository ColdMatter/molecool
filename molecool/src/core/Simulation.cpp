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
        while (true) {

        }
    }

}