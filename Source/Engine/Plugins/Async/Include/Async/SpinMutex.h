//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Atomic.h>
#include "Async.h"

namespace Djinn {
    namespace Async {
        /// Spin Lock based Mutex
        class SpinMutex {
        public:
            SpinMutex() = default;

            SpinMutex(const SpinMutex&) : locked(false) {

            }

            /// Lock this value
            void Lock() {
                while (!locked.CmpExchange(false, true)) Yield();
            }

            /// Unlock this value
            void Unlock() {
                locked = false;
            }

        private:
            Core::Atomic<bool> locked{false};
        };
    }
}