#pragma once

#include <chrono>
#include "Log.h"

struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> start,end;
    std::chrono::duration<double> duration;

    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        double durationInMs = duration.count() * 1000.0f;
        MC_CORE_TRACE("Timer took {0} ms", durationInMs);
    }
};