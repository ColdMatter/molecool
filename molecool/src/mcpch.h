#pragma once

// common standard library headers
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <array>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>     // size_t, EXIT_SUCCESS, etc.
#include <time.h>
#include <chrono>
#include <cmath>        // standard and special math functions
#include <assert.h>

// high-performance library headers
#include <omp.h>
#include <mkl.h>

// boost library headers
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/openmp/openmp.hpp>
#include <boost/numeric/odeint/external/mkl/mkl_operations.hpp>

// project headers that shouldn't change very often
#include "core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "debug/Profiler.h"
