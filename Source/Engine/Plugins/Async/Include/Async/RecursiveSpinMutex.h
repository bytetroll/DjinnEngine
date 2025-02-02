//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Atomic.h>
#include <Platform/ThreadAPI.h>
#include "Async.h"

namespace Djinn {
    namespace Async {
        /// Spin Lock based Recursive Mutex
        class RecursiveSpinMutex {
        public:
            RecursiveSpinMutex() = default;

            RecursiveSpinMutex(const RecursiveSpinMutex&) : lockCount{0}, owner{} {

            }

            /// Lock this mutex
            void Lock() {
                auto tid = Platform::ThreadAPI::GetCurrentID();
                if (owner.Load() != tid) {
                    while (!owner.CmpExchange({}, tid)) Yield();
                }
                lockCount++;
            }

            /// Unlock this mutex
            void Unlock() {
                if (--lockCount == 0) {
                    owner = {};
                }
            }

        private:
            Core::Atomic<Platform::ThreadAPI::ThreadID> owner;
            Core::AtomicCounter lockCount{0};
        };
    }
}