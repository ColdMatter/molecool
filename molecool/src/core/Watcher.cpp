#include "mcpch.h"
#include "Watcher.h"

namespace molecool {

    Watcher::Watcher(const Ensemble& ens)
        : ensemble(ens)
    {}

    void Watcher::deployObservers(const Ensemble& ens, double t) {
        MC_PROFILE_FUNCTION();
        for (const auto& obs : observers) {
            (*obs)(ens, t);
        }
    }

    void Watcher::addObserver(ObserverPtr obs) {
        MC_CORE_TRACE("Adding observer");
        observers.push_back(obs);
    }


}
