#pragma once

/*
This is a collection of classes useful for generating random numbers.
Currently, the structure is built on top of the Intel MKL vector statistics library (vsl)
random number generators. 

The model for the random number generation class structure is to use a shared_ptr to
manage the lifetime of the random stream to ensure it lives at least as long as the distributions
that use it. 

Usage examples of Dist class:


1) single-threaded version:
    double a[10];                                           // target array for random values
    auto dist = Dist(PDF::gaussian, 0, 1);                  // create distribution
    auto sp = std::make_shared<RandomStream>(int seed);     // acquire stream, optional seed
    dist.sample(sp, 10, a);                                 // sample the distribution, filling target vector
    

2) Multi-threaded example, requires building a vector of shared_ptrs before entering parallel region

    int seed = (int)time(0);
    std::vector<std::shared_ptr<RandomStream>> sps;
    for (int i = 0; i < 2; ++i) {
        sps.push_back(std::make_shared<RandomStream>(seed));
    }
    #pragma omp parallel for 
    {
        int threadId = omp_get_thread_num();
        double b[10];
        auto dist = Dist(PDF::gaussian, 0, 1);
        dist.sample(sps[threadId], 10, b);
    }

*/

#include <mkl.h>
#include <time.h>
#include <memory>
#include "Core.h"

namespace molecool {

    // a wrapper class for MKL random number streams 
    // The BRNG choice is restricted to be MT2203
    // each class instance gets an independent stream
    // this class of BRNG can provide up to 6024 independent streams
    class  RandomStream {

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

    // Random number generator types
    enum class Rng_t { MCG31, R250, MRG32K3A, MCG59, MT19937, MT2203, SFMT19937, SOBOL, NIEDERR };

    // supported distribution types (PDFs)
    // currently only support MKL distributions that take two parameters
    enum class  PDF {
        delta,           // not really a pdf, used for default arguments -> always gives 0.0, or always gives p1
        flat,           // parameters are min, max [p1,p2)
        gaussian,       // parameters are mean, sigma (Gaussian width)
        exponential,    // parameters are displacement, scale factor
        laplace,        // parameters are mean, scale factor
        cauchy,         // parameters are displacement, scale factor
        rayleigh,       // parameters are displacement, scale factor
        gumbel          // parameters are displacement, scale factor
    };


    /*
    The Dist class represents a (continuous) distribution of random numbers, 
    from which the caller can sample values by providing a RandomStream. 

    One thing to try and improve is the unnecessary shared_ptr reference counting when passing the 
    pointer between in-class methods.  I'm not sure how to get around this.  If you pass by reference 
    then the same method won't work as intended when used externally (because we want external calls 
    to add to the reference count).  It's probably OK for now.  
    */
    class  Dist {

    public:
        Dist();
        Dist(PDF PDF, double p1 = 0.0, double p2 = 0.0);

        // sample the distribution and place results in target vector
        int sample(std::shared_ptr<RandomStream> sp, int nValues, std::vector<double>& target, int indexOffset = 0) const;

        double getPeak() const;

        // allow dynamic updating of Pdf parameters (for instance, to allow for time-dependent distribution sampling)
        void changePdfParameters(double p1, double p2);

    private:

        PDF m_pdf;
        double m_p1, m_p2;
        bool m_pdfDefined = false;

    };

    using PosDist = Dist;
    using VelDist = Dist;

}



