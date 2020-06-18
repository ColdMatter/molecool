#pragma once

#include <mkl.h>
#include <time.h>
#include "Core.h"

namespace molecool {

// a wrapper class for MKL random number streams 
// The BRNG choice is restricted to be MT2203
// this class of BRNG can provide up to 6024 independent streams
    class MOLECOOL_API MklRandomStream {

    public:
        MklRandomStream(int seed = (int)time(0));

        ~MklRandomStream();

        VSLStreamStatePtr getStream();

    private:

        VSLStreamStatePtr stream = nullptr;
        const int maxStreamId = 6024;
        static int streamId;

        void checkStatus(int status);
    };


}



