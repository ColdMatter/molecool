#pragma once

#include <iostream>
#include <sstream>
#include <string>

// macros for dynamic library imports/exports (required on Windows)
// see https://sourceforge.net/p/predef/wiki/OperatingSystems/ for standard macros
#ifdef _WIN32
    //  Windows (both 32 and 64 bit)
    #ifdef MC_BUILD_DLL
        // building dll
        #define MOLECOOL_API __declspec(dllexport)
    #else
        #define MOLECOOL_API __declspec(dllimport)
    #endif
#else
    //  Linux or OSX don't require special import/export symbols
    #define MOLECOOL_API
#endif

namespace molecool {

    // engine version reporting.  Note - constexpr functions are always 'inline' 
    // so no multiply-defined issues when header is included multiple places
    constexpr std::string_view engineVersion = "0.0.0";
    MOLECOOL_API constexpr std::string_view getEngineVersion() { return engineVersion; }

}

