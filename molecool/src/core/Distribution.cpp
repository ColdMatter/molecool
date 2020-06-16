#include "Distribution.h"
#include <omp.h>

namespace molecool {

    Distribution::Distribution(DistributionType distType, double p1, double p2) 
    : m_distType(distType), m_p1(p1), m_p2(p2)
    {

    }

    int Distribution::sample(VSLStreamStatePtr rngStream, int nValues, double* target) {
        return sample(rngStream, nValues, target, m_p1, m_p2);
    }

    int Distribution::sample(VSLStreamStatePtr rngStream, int nValues, double* target, double p1, double p2) {
        switch (m_distType) {
        case DistributionType::flat:
            return vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, rngStream, nValues, target, p1, p2);
        case DistributionType::gaussian:
            return vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, rngStream, nValues, target, p1, p2);
        case DistributionType::exponential:
            return vdRngExponential(VSL_RNG_METHOD_EXPONENTIAL_ICDF, rngStream, nValues, target, p1, p2);
        case DistributionType::laplace:
            return vdRngLaplace(VSL_RNG_METHOD_LAPLACE_ICDF, rngStream, nValues, target, p1, p2);
        case DistributionType::cauchy:
            return vdRngCauchy(VSL_RNG_METHOD_CAUCHY_ICDF, rngStream, nValues, target, p1, p2);
        case DistributionType::rayleigh:
            return vdRngRayleigh(VSL_RNG_METHOD_RAYLEIGH_ICDF, rngStream, nValues, target, p1, p2);
        default:
            return -1;
        }
    }
}
