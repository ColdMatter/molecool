#pragma once

#include "Core.h"
#include "Ensemble.h"
#include "Thruster.h"

namespace molecool {

    class  Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();

        // each Simulation should get an Environment (singleton), Ensemble, a Thruster (propagator), etc.
        // Environment environment;
  

        Ensemble ensemble;
        Thruster thruster;

    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}