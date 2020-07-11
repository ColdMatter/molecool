#include "mcpch.h"
#include "Random.h"

namespace molecool {

    // initialize static member(s)
    int RandomStream::s_streamId = 0;

    RandomStream::RandomStream(int seed) {
        //MC_CORE_TRACE("Creating new MT2203 random stream with id {0} and seed {1}", s_streamId, seed);
        int status = vslNewStream(&m_stream, VSL_BRNG_MT2203 + s_streamId, seed);
        checkStatus(status);
        if (s_streamId < maxStreamId - 1) { m_id = s_streamId++; }
        else { m_id = s_streamId;  MC_CORE_WARN("MT2203 BRNG maximum stream count exceeded"); }
    }

    RandomStream::~RandomStream() {
        //MC_CORE_TRACE("Destroying random stream with id {0}", m_id);
        vslDeleteStream(&m_stream);
    }

    VSLStreamStatePtr RandomStream::getStream() { return m_stream; }

    // TODO: report actual error using magic_enum?
    void RandomStream::checkStatus(int status) {
        if (status) { MC_CORE_ERROR("stream generation failed"); }
    }

//////////////////////////////////////////////////////////////////////////////////////////////

    Dist::Dist()
        : m_pdf(PDF::delta), m_p1(0.0), m_p2(0.0)
    {}

    Dist::Dist(PDF pdf, double p1, double p2)
        : m_pdf(pdf), m_p1(p1), m_p2(p2), m_pdfDefined(true)
    {}

    int Dist::sample(std::shared_ptr<RandomStream> sp, int nValues, std::vector<double>& target, int offset) const
    {
        // grow the target vector if needed
        size_t minTargetSize = nValues + offset;
        if (target.size() < minTargetSize) {
            MC_CORE_WARN("Random number target vector too small, resizing");
            target.resize(minTargetSize);
        }

        // fill with random numbers from stream
        if (!m_pdfDefined) {
            MC_CORE_WARN("Using default 'delta' distribution, all sample variates will be equal");
        }
        double* tarPtr = &target[offset];
        VSLStreamStatePtr rngStream = sp->getStream();
        switch (m_pdf) {
        case PDF::delta:
            std::fill_n(tarPtr, nValues, m_p1);
            return EXIT_SUCCESS;
        case PDF::flat:
            return vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, rngStream, nValues, tarPtr, m_p1, m_p2);
        case PDF::gaussian:
            return vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF, rngStream, nValues, tarPtr, m_p1, m_p2);
        case PDF::exponential:
            return vdRngExponential(VSL_RNG_METHOD_EXPONENTIAL_ICDF, rngStream, nValues, tarPtr, m_p1, m_p2);
        case PDF::laplace:
            return vdRngLaplace(VSL_RNG_METHOD_LAPLACE_ICDF, rngStream, nValues, tarPtr, m_p1, m_p2);
        case PDF::cauchy:
            return vdRngCauchy(VSL_RNG_METHOD_CAUCHY_ICDF, rngStream, nValues, tarPtr, m_p1, m_p2);
        case PDF::rayleigh:
            return vdRngRayleigh(VSL_RNG_METHOD_RAYLEIGH_ICDF, rngStream, nValues, tarPtr, m_p1, m_p2);
        default:
            return -1;
        }

    }

    void Dist::changePdfParameters(double p1, double p2)
    {
        m_p1 = p1;
        m_p2 = p2;
    }

    // return the position of the peak of the distribution
    double Dist::getPeak() const {
        switch (m_pdf) {
        case PDF::delta:
            return m_p1;
        case PDF::flat:
            return (m_p2 - m_p1) / 2;
        case PDF::gaussian:
            return m_p1;
        case PDF::exponential:
            return m_p1;
        case PDF::laplace:
            return m_p1;
        case PDF::cauchy:
            return m_p1;
        case PDF::rayleigh:
            return m_p1 + m_p2;
        default:
            return 0.0;
        }
    }


}
