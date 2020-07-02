#pragma once

namespace molecool {

    // engine version reporting.  Note - constexpr functions are always 'inline' 
    // so no multiply-defined issues when header is included multiple places
    constexpr char* engineVersion = "0.0.0";
    constexpr char* getEngineVersion() { return engineVersion; }

}

