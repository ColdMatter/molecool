#pragma once

/*
This class represents a continuous 1D distribution that generates random numbers 
using a MKL VSL stream.  There are two common usages:

double targetArray[N];              // create target array
VSLStreamStatePtr stream;           // create pointer to stream        
VSLNewStream(&stream, BRNG, seed);  // create stream

usage (1) -- for constant distribution parameters, give parameters when instantiating.  
    Distribution dist = Distribution(style, p1, p2);    // create the distribution
    dist.sample(&stream, N, targetArray);               // get some random numbers

usage (2) -- for distribution parameters that vary between calls, pass parameters at call time
    Distribution dist = Distribution(style);            // create the distribution
    dist.sample(&stream, N, targetArray, p1, p2);       // get some random numbers with these parameters
*/

#include <mkl.h>
#include "Core.h"

namespace molecool {

    // Random number generator types
    enum class RngType {MCG31, R250, MRG32K3A, MCG59, MT19937, MT2203, SFMT19937, SOBOL, NIEDERR};

    // supported distribution types (shapes)
    // currently only support MKL distributions that take two parameters
    enum class Shape {
        flat,           // parameters are min, max [p1,p2)
        gaussian,       // parameters are mean, sigma (Gaussian width)
        exponential,    // parameters are displacement, scale factor
        laplace,        // parameters are mean, scale factor
        cauchy,         // parameters are displacement, scale factor
        rayleigh,       // parameters are displacement, scale fator
        gumbel          // parameters are displacement, scale factor
    };
    
    class MOLECOOL_API Distribution {

    public:
        // constructor, takes distribution shape and 2 parameters (e.g. center and width)
        Distribution(Shape shape, double p1 = 0, double p2 = 0);

        // sample the distribution
        int sample(VSLStreamStatePtr rngStream, int nValues, double* target);
        int sample(VSLStreamStatePtr rngStream, int nValues, double* target, double p1, double p2);


    private:

        Shape m_shape = Shape::flat;  

        // shape parameters
        double m_p1 = 0;
        double m_p2 = 0;     

    };

}


