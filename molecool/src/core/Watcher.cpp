#include "mcpch.h"
#include "Watcher.h"

namespace molecool {

    Watcher::Watcher(const Ensemble& ens)
        : ensemble(ens)
    {}

    void Watcher::deploy(double t) {
        MC_PROFILE_FUNCTION();
        for (Observer obs : observers) {
            obs(ensemble, t);
        }
    }

    void Watcher::addObserver(Observer obs) {
        observers.push_back(obs);
    }


}
