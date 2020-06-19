#include <time.h>
#include "Log.h"
#include "Random.h"

namespace molecool {

    // initialize static member(s)
    int RandomStream::s_streamId = 0;

    RandomStream::RandomStream(int seed) {
        MC_CORE_TRACE("Creating new MT2203 random stream with id {0} and seed {1}", s_streamId, seed);
        int status = vslNewStream(&m_stream, VSL_BRNG_MT2203 + s_streamId, seed);
        checkStatus(status);
        if (s_streamId < maxStreamId - 1) { m_id = s_streamId++; }
        else { m_id = s_streamId;  MC_CORE_WARN("MT2203 BRNG maximum stream count exceeded"); }
    }

    RandomStream::~RandomStream() {
        MC_CORE_TRACE("Destroying random stream with id {0}", m_id);
        vslDeleteStream(&m_stream);
    }

    VSLStreamStatePtr RandomStream::getStream() { return m_stream; }

    // TODO: report actual error using magic_enum?
    void RandomStream::checkStatus(int status) {
        if (status) { MC_CORE_ERROR("stream generation failed"); }
    }

//////////////////////////////////////////////////////////////////////////////////////////////

    ContinuousDistribution::ContinuousDistribution(std::shared_ptr<RandomStream> sp, Shape shape, double p1, double p2)
        : m_streamSp(sp), m_shape(shape), m_p1(p1), m_p2(p2)
    {}

    int ContinuousDistribution::sample(int nValues, double* target) {
        return sample(nValues, target, m_p1, m_p2);
    }

    int ContinuousDistribution::sample(int nValues, double* target, double p1, double p2) {
        VSLStreamStatePtr rngStream = m_streamSp->getStream();
        switch (m_shape) {
        case Shape::flat:
            return vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, rngStream, nValues, target, p1, p2);
        case Shape::gaussian:
            return vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, rngStream, nValues, target, p1, p2);
        case Shape::exponential:
            return vdRngExponential(VSL_RNG_METHOD_EXPONENTIAL_ICDF, rngStream, nValues, target, p1, p2);
        case Shape::laplace:
            return vdRngLaplace(VSL_RNG_METHOD_LAPLACE_ICDF, rngStream, nValues, target, p1, p2);
        case Shape::cauchy:
            return vdRngCauchy(VSL_RNG_METHOD_CAUCHY_ICDF, rngStream, nValues, target, p1, p2);
        case Shape::rayleigh:
            return vdRngRayleigh(VSL_RNG_METHOD_RAYLEIGH_ICDF, rngStream, nValues, target, p1, p2);
        default:
            return -1;
        }
    }

    // single-sample getter, uses class-level buffer to interface with vectorized MKL stream structure
    double ContinuousDistribution::sample() {
        if (m_bufferPtr == m_buffer.data()) {
            // buffer is empty, fill it
            int nValues = (int)m_buffer.size();
            sample(nValues, m_buffer.data());
            m_bufferPtr += nValues;
        }
        m_bufferPtr--;                      
        double result = *m_bufferPtr;
        return result;
    }


}
