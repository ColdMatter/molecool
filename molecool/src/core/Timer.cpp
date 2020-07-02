#include "mcpch.h"
#include "Timer.h"

namespace molecool {

    /////////////////////////////////////////////////////////////////////////

    Timer::Timer(const char* name)
        : m_name(name), m_stopped(false)
    {
        m_startTimepoint = std::chrono::high_resolution_clock::now();
    }
    
    Timer::~Timer() {
        if (!m_stopped) {
            stop();
        }
    }

    void Timer::stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        m_stopped = true;
        double duration = end - start;   // in us
        TimerManager::getInstance()->report(m_name, duration);
        //MC_CORE_TRACE("{0} timer took {1} us", m_name, duration);
    }

    /////////////////////////////////////////////////////////////////////////

    TimerReport::TimerReport(const char* name, double time)
        : m_name(name)
    {
        addData(time);
    }

    /////////////////////////////////////////////////////////////////////////
    
    // define static member(s)
    std::vector< molecool::TimerReport > molecool::TimerManager::s_timerReports;

    TimerManager::TimerManager() {

    }

    void TimerManager::report(const char* timerName, double time) {
        auto it = std::find_if(s_timerReports.begin(), s_timerReports.end(), [&timerName](const TimerReport& obj) {return obj.m_name == timerName; });
        if (it != s_timerReports.end()) {
            // this timer has previously reported, add the data value
            it->addData(time);
        }
        else {
            // first time this timer has reported a value
            s_timerReports.push_back(TimerReport(timerName, time));
        }
    }

    void TimerManager::summary() {
        for (TimerReport tr : s_timerReports) {
            double duration = tr.getMean();
            double div = 1.0;
            char* unit = "us";
            if (duration > 1e6) {
                unit = "s";
                div = 1e6;
            }
            else if (duration > 1e3) {
                unit = "ms";
                div = 1e3;
            }
            MC_CORE_INFO("{0} timer called {1} times, (av, min, max, tot) = ({2:.3f}, {3}, {4}, {5}) {6}",
                tr.m_name, tr.getCount(), tr.getMean() / div, tr.getMin() / div, tr.getMax() / div, tr.getSum() / div, unit);
        }
    }
}