#pragma once

/*
This is a scoped timer that measures the time between instantiation 
and when the timer object goes out of scope.  The constructor takes 
and optional std::string name parameter for clarity of use.

Usage is generally as follows.  One option is to instantiate the 
timer in a function that is to be timed:
void someFunction() 
{                           // begin function scope
    ScopedTimer timer();    // instantiate timer, timing starts
    ***CODE TO TIME***      // function actions to be timed
}                           // timer destroyed when leaving function scope, timing stops

Alternatively, you can always create a local scope with {}:
{                           // start local scope
    ScopedTimer timer();    // instantiate timer, timing starts 
    ***CODE TO TIME***      // scoped actions to be timed
}                           // timer destroyed when leaving local scope, timing stops
*/

#include "mcpch.h"

struct ScopedTimer {
    std::chrono::time_point<std::chrono::steady_clock> start,end;
    std::chrono::duration<double> duration;
    std::string m_name;

    ScopedTimer(const std::string& timerName = "") {
        m_name = timerName;
        start = std::chrono::high_resolution_clock::now();
    }

    ~ScopedTimer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        double durationInMs = duration.count() * 1000.0f;
        MC_CORE_TRACE("{0} took {1} ms", m_name, durationInMs);
    }
};