#pragma once

#include "Core.h"
#include "Ensemble.h"
#include "Thruster.h"
#include "Watcher.h"

#include "LuaScript.h"
#include "sol/sol.hpp"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

namespace molecool {

    class  Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();

        // helper methods for hiding class structure from user
        void addParticles(int n, ParticleId p, PosDist xDis = Dist(), VelDist vxDis = Dist(), PosDist yDis = Dist(), VelDist vyDis = Dist(), PosDist zDis = Dist(), VelDist vzDis = Dist());
        void addFilter(FilterFunction ff);
        void addForce(ForceFunction ff);
        void addObserver(ObserverPtr obs);

        sol::state lua;
        Ensemble ensemble;
        Thruster thruster;
        Watcher watcher;

        // simulation time control
        double tStart = 0.0;
        double tEnd = 1.0;
        double dt = 0.001;

    private:

        void propagate();
        void parseScript();

        Dist extractDist(LuaScript& script, std::string name);
        Dist extractDist(sol::table table);
        PDF nameToPDF(std::string name);

    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}