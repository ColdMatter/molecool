#include <time.h>
#include "Log.h"
#include "Random.h"

namespace molecool {

    // initialize static member(s)
    int RandomStream::s_streamId = 0;

    RandomStream::RandomStream(int seed) {
        MC_CORE_TRACE("Creating new MT2203 stream with id {0} and seed {1}", s_streamId, seed);
        int status = vslNewStream(&m_stream, VSL_BRNG_MT2203 + s_streamId, seed);
        checkStatus(status);
        if (s_streamId < maxStreamId - 1) { m_id = s_streamId++; }
        else { m_id = s_streamId;  MC_CORE_WARN("MT2203 BRNG stream count exceeded"); }
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


}
