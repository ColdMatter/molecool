#pragma once

#include "Core.h"

namespace molecool {

    class  Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();
    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}