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

        // helper method to add particles to the ensemble
        // omitted distributions will result in that dimension being all 0.0s
        void addParticles(size_t n, ParticleId p, PosDist xDis = Dist(), VelDist vxDis = Dist(), PosDist yDis = Dist(), VelDist vyDis = Dist(), PosDist zDis = Dist(), VelDist vzDis = Dist());

        Ensemble ensemble;
        Thruster thruster;

    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}