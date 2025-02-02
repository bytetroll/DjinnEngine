#pragma once

#ifdef DJINN_PLATFORM_WINDOWS
#include <Core/Windows.h>
#endif

#ifdef DJINN_PLATFORM_LINUX
#include <time.h>
#endif

namespace Djinn {
    namespace Core {
        class Timer {
        public:
            Timer();

            /// Get delta from last stamp
            /// \return
            double Delta();

            /// Get delta from last stamp, no reset
            /// \return
            double DeltaRepeat();

        private:
#ifdef DJINN_PLATFORM_WINDOWS
            LARGE_INTEGER frequency;
            LARGE_INTEGER t1, t2;
#endif
#ifdef DJINN_PLATFORM_LINUX
            struct timespec t1;
#endif
        };
    }
}