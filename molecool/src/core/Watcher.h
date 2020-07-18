#pragma once

#include "Ensemble.h"

namespace molecool {



    class Observer {
    public:
        virtual void operator()(const Ensemble& ens, double t) = 0;	// pure virtual, must be implemented in child classes
    };

    //using Observer = std::function< void(const Ensemble& e /*ensemble*/, double /*t*/) >;
    using ObserverPtr = std::shared_ptr<Observer>;
    //using Observer = std::shared_ptr<void(const Ensemble&, double)>;    // shared_ptr to function
    
    class Watcher
    {
    public:
        Watcher(const Ensemble& ens);

        void deployObservers(double t);

        void addObserver(ObserverPtr obs);

    private:

        const Ensemble& ensemble;

        // a collection of observers
        std::vector<ObserverPtr> observers;

    };
}


