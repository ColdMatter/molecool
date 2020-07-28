#pragma once

#include "Core.h"
#include "Ensemble.h"
#include "Thruster.h"
#include "Watcher.h"
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

        // a template for registering derived classes of Observer with Lua, to be called from/during user simulation constructor
        // here the usertype is created using a sol::factory, i.e. a generating function that returns a smart pointer
        // in this case, Lua shouldn't actually do the allocation, C++ allocates the memory and has full ownership
        template <class C, typename ...Args>
        void registerObserver(std::string name, ObserverPtr(*facFunc)(Args... args)) {
            lua.new_usertype<C>(name, sol::call_constructor, sol::factories(facFunc));
        }

        // simulation time control
        double tStart = 0.0;
        double tEnd = 1.0;
        double dt = 0.001;

        sol::state lua;
        Ensemble ensemble;
        Thruster thruster;
        Watcher watcher;

    private:

        void propagate();
        void setupScript();
        void parseScript();

        Dist extractDist(sol::table table);
        PDF nameToPDF(std::string name);

    };

    // METHOD TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}