#pragma once

#include "Core.h"

namespace molecool {

    class  Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();

        // each Simulation should get an Environment (singleton), Ensemble, a Thruster, a Propagator, etc.
        // do we need a vector of ensembles for different species? or allow multiple species to be added to an ensemble?
        // Environment
        // Ensemble ensemble();
        // Thruster thruster();
        // Propagator propagator();
    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}