#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace molecool {

    // define static members
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;

    void Log::init() {
        // timestamp, logger name, message
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        s_coreLogger = spdlog::stdout_color_mt("MOLECOOL");    // engine logger
        s_coreLogger->set_level(spdlog::level::trace);
        
        s_clientLogger = spdlog::stdout_color_mt("SIM");       // simulation logger
        s_clientLogger->set_level(spdlog::level::trace);
    }

}