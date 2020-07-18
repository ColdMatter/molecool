#pragma once

// THIS HEADER IS INTENDED FOR INCLUSION BY CLIENTS/APPLICATIONS ONLY

#include "core/Core.h"

//---------- Base class for client simulations -------------
#include "core/Simulation.h"
//----------------------------------------------------------

//--- Engine (core) and client simulation logging ----------
#include "core/Log.h"
//----------------------------------------------------------

//--- Timing -----------------------------------------------
#include "debug/Profiler.h"
//----------------------------------------------------------

#include "assets/observers/Trajectorizer.h"

//--- Random number generation -----------------------------
#include "core/Random.h"
//----------------------------------------------------------

#include "core/Vector.h"

//--- Particle ensemble ------------------------------------
#include "core/Ensemble.h"
//----------------------------------------------------------

//--- Entry point [main()] for client simulation -----------
// main() is defined here
#include "core/EntryPoint.h"
//----------------------------------------------------------
