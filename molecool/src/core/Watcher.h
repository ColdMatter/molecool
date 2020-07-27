#pragma once

#include "Ensemble.h"

namespace molecool {

    class Observer;                                 // forward declaration
    using ObserverPtr = std::shared_ptr<Observer>;  // to allow us to write this

    class Observer {
    public:
        virtual ~Observer() = default;
        virtual void operator()(const Ensemble& ens, double t) = 0;	    // pure virtual, must be implemented in child classes
    };


    
    class Watcher
    {
    public:
        Watcher(const Ensemble& ens);

        void deployObservers(const Ensemble& ens, double t);

        void addObserver(ObserverPtr obs);

    private:

        const Ensemble& ensemble;

        // a collection of observers
        std::vector<ObserverPtr> observers;

    };
}


