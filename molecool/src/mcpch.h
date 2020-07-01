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

// high-performance library headers
#include <omp.h>
#include <mkl.h>

// project headers that shouldn't change very often
#include "core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
