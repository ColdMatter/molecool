#include <time.h>
#include "Log.h"
#include "Random.h"

namespace molecool {

    // initialize static member(s)
    int MklRandomStream::streamId = 0;

    MklRandomStream::MklRandomStream(int seed) {
        MC_CORE_TRACE("Creating new VSL MT2203 stream with id {0} and seed {1}", streamId, seed);
        int status = vslNewStream(&stream, VSL_BRNG_MT2203 + streamId, seed);
        checkStatus(status);
        if (streamId < maxStreamId - 1) { streamId++; }
        else { MC_CORE_WARN("MT2203 BRNG at stream count limit"); }
    }

    MklRandomStream::~MklRandomStream() {
        MC_CORE_TRACE("Deleting MKL random stream");
        vslDeleteStream(&stream);
    }

    VSLStreamStatePtr MklRandomStream::getStream() { return stream; }

    // TODO: report actual error using magic_enum?
    void MklRandomStream::checkStatus(int status) {
        if (status) { MC_CORE_ERROR("stream generation failed"); }
        else { MC_CORE_TRACE("stream generation succeeded"); }
    }


}
