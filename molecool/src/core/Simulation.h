#pragma once

#include "Core.h"
#include "Ensemble.h"
#include "Thruster.h"
#include "Watcher.h"

namespace molecool {

    class  Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();
        void propagate();

        // each Simulation should get an Environment (singleton), Ensemble, a Thruster (propagator), etc.
        // Environment environment;

        // helper methods for hiding class structure from user
        void addParticles(int n, ParticleId p, PosDist xDis = Dist(), VelDist vxDis = Dist(), PosDist yDis = Dist(), VelDist vyDis = Dist(), PosDist zDis = Dist(), VelDist vzDis = Dist());
        void addFilter(FilterFunction ff);
        void addForce(ForceFunction ff);
        void addObserver(Observer obs);

        Ensemble ensemble;
        Thruster thruster;
        Watcher watcher;

    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}