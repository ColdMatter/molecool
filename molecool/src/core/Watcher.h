#pragma once

#include "Ensemble.h"

namespace molecool {

    using Observer = std::function< void( Ensemble& e /*ensemble*/, double /*t*/) >;
    
    class Watcher
    {
    public:
        Watcher(Ensemble& ens);

        void deploy(double t);

        void addObserver(Observer obs);

    private:

        Ensemble& ensemble;

        // a collection of filter functions that return true if a particle should be stopped
        std::vector<Observer> observers;

    };
}


