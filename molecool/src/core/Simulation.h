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

        // helper methods for hiding class structure from user
        void addParticles(int n, ParticleId p, PosDist xDis = Dist(), VelDist vxDis = Dist(), PosDist yDis = Dist(), VelDist vyDis = Dist(), PosDist zDis = Dist(), VelDist vzDis = Dist());
        void addFilter(FilterFunction ff);
        void addForce(ForceFunction ff);
        void addObserver(ObserverPtr obs);

        Ensemble ensemble;
        Thruster thruster;
        Watcher watcher;

        // simulation time control
        double tStart = 0.0;
        double tEnd = 1.0;
        double dt = 0.001;

    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}