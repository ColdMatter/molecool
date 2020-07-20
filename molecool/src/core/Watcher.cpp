#include "mcpch.h"
#include "Watcher.h"

namespace molecool {

    Watcher::Watcher(const Ensemble& ens)
        : ensemble(ens)
    {}

    void Watcher::deployObservers(double t) {
        MC_PROFILE_FUNCTION();
        for (const auto& obs : observers) {
            (*obs)(ensemble, t);
        }
    }

    void Watcher::addObserver(ObserverPtr obs) {
        observers.push_back(obs);
    }


}
