#pragma once

/*
This is a collection of classes useful for generating random numbers.
Currently, the structure is built on top of the Intel MKL vector statistics library (vsl)
random number generators. 

The model for the random number generation class structure is to use a shared_ptr to
manage the lifetime of the random stream to ensure it lives at least as long as the distributions
that use it.  To ensure that happens, xyzDistribution objects are passed the shared_ptr by value
so that the reference count is correctly 

Usage examples of ContinuousDistribution class:

1) single-threaded version:
    double a[10];                                                // target array for random values
    auto dist = ContinuousDistribution(Shape::gaussian, 0, 1);   // create distribution
    auto sp = std::make_shared<RandomStream>(int seed);          // acquire stream, optional seed
    dist.sample(sp, 10, a);                                      // sample the distribution, filling array
    double singleSample = dist.sample(sp);                       // just grab a single sample (slower)
    
2) Multi-threaded example, requires building a vector of shared_ptrs before entering parallel region
    const int nThreads = 2;
    int seed = (int)time(0);
    std::vector<std::shared_ptr<RandomStream>> sps;
    for (int i = 0; i < nThreads; ++i) {
        sps.push_back(std::make_shared<RandomStream>(seed));
    }
    omp_set_num_threads(nThreads);
    #pragma omp parallel 
    {
        int threadId = omp_get_thread_num();
        double b[10];
        auto dist = ContinuousDistribution(Shape::gaussian, 0, 1);
        dist.sample(sps[threadId], 10, b);
    }

*/

#include <mkl.h>
#include <time.h>
#include "Core.h"
#include "Distribution.h"

namespace molecool {

// a wrapper class for MKL random number streams 
// The BRNG choice is restricted to be MT2203
// this class of BRNG can provide up to 6024 independent streams
    class MOLECOOL_API RandomStream {

    public:
        RandomStream(int seed = (int)time(0));
        ~RandomStream();
        VSLStreamStatePtr getStream();

    private:

        VSLStreamStatePtr m_stream = nullptr;
        const int maxStreamId = 6024;
        static int s_streamId;
        int m_id;
        void checkStatus(int status);
    };
/*
    // Random number generator types
    enum class RngTypey { MCG31, R250, MRG32K3A, MCG59, MT19937, MT2203, SFMT19937, SOBOL, NIEDERR };

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
*/
    class MOLECOOL_API ContinuousDistribution {

    public:
        // constructor, takes shared_ptr to random stream, distribution shape and 2 parameters (e.g. center and width)
        // shared_ptr is passed by value to increment the reference count to make this object a shared owner
        ContinuousDistribution(std::shared_ptr<RandomStream> sp, Shape shape, double p1 = 0, double p2 = 0);

        // sample the distribution, vectorized
        int sample(int nValues, double* target);
        int sample(int nValues, double* target, double p1, double p2);

        // sample the distribution, single values (uses class buffer)
        double sample();                        // uses shape parameters passed at construction

    private:

        std::shared_ptr<RandomStream> m_streamSp = nullptr;

        // shape parameters
        Shape m_shape = Shape::flat;
        double m_p1 = 0;
        double m_p2 = 0;

        std::array<double, 1024> m_buffer;
        double* m_bufferPtr = m_buffer.data();

    };


}



