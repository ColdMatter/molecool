#pragma once

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

