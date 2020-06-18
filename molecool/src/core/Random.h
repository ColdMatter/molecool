#pragma once

/*
This is a collection of classes useful for generating random numbers.
Currently, the structure is built on top of the Intel MKL vector statistics library (vsl)
random number generators.  

*/

#include <mkl.h>
#include <time.h>
#include "Core.h"

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


}



