#pragma once

#include <string>

// macros for dynamic library imports/exports (required on Windows only)
// see https://sourceforge.net/p/predef/wiki/OperatingSystems/ for standard macros

namespace molecool {

    // engine version reporting.  Note - constexpr functions are always 'inline' 
    // so no multiply-defined issues when header is included multiple places
    constexpr std::string_view engineVersion = "0.0.0";
    constexpr std::string_view getEngineVersion() { return engineVersion; }

}

