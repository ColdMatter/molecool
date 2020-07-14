#pragma once

#include "Ensemble.h"

namespace molecool {

    using Observer = std::function< void(const Ensemble& e /*ensemble*/, double /*t*/) >;
    
    class Watcher
    {
    public:
        Watcher(const Ensemble& ens);

        void deploy(double t);

        void addObserver(Observer obs);

    private:

        const Ensemble& ensemble;

        // a collection of filter functions that return true if a particle should be stopped
        std::vector<Observer> observers;

    };
}


