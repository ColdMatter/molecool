#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

// core logging macros
#define MC_CORE_FATAL(...) molecool::Log::getCoreLogger()->critical(__VA_ARGS__)
#define MC_CORE_ERROR(...) molecool::Log::getCoreLogger()->error(__VA_ARGS__)
#define MC_CORE_INFO(...)  molecool::Log::getCoreLogger()->info(__VA_ARGS__)
#define MC_CORE_WARN(...)  molecool::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MC_CORE_TRACE(...) molecool::Log::getCoreLogger()->trace(__VA_ARGS__)

// client logging macros
#define MC_FATAL(...) molecool::Log::getClientLogger()->critical(__VA_ARGS__)
#define MC_ERROR(...) molecool::Log::getClientLogger()->error(__VA_ARGS__)
#define MC_INFO(...)  molecool::Log::getClientLogger()->info(__VA_ARGS__)
#define MC_WARN(...)  molecool::Log::getClientLogger()->warn(__VA_ARGS__)
#define MC_TRACE(...) molecool::Log::getClientLogger()->trace(__VA_ARGS__)

namespace molecool {

    class MOLECOOL_API Log {
    public:
        static void init();

        // get logger methods, return shared_ptr by reference to avoid unnecessary reference counting on copy
        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };

}
