//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once


#include <Core/Types.h>
#include <Core/Common.h>
#include <Core/Atomic.h>
#include <atomic>

namespace Djinn {
    namespace Platform {
        // Barrier
        class SpinBarrier {
        public:
            SpinBarrier() {

            }

            DJINN_INLINE void SetThreadCount(UInt32 threadCount) {
                this->threadCount = threadCount;
            }

            DJINN_INLINE void Wait() {
                Int64 copy = passed.Load(Core::AtomicOperation::RELAXED);
                if (bar++ == (threadCount - 1)) {
                    bar = 0;
                    passed.Store(copy + 1, Core::AtomicOperation::RELEASE);
                } else {
                    while (passed.Load(Core::AtomicOperation::RELAXED) == copy);
                    std::atomic_thread_fence(std::memory_order_acquire);
                }
            }

        private:
            UInt32 threadCount = 0;
            Core::AtomicCounter bar;
            Core::AtomicCounter passed;
        };
    }
}