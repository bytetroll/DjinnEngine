//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Atomic.h>
#include <Platform/ThreadAPI.h>

namespace Djinn {
    namespace Platform {
        /// Spin Lock based Recursive Mutex
        class RecursiveSpinMutex {
        public:
            RecursiveSpinMutex() = default;

            RecursiveSpinMutex(const RecursiveSpinMutex&) : lockCount{0}, owner{} {

            }

            /// Lock this mutex
            void Lock() {
                auto tid = ThreadAPI::GetCurrentID();
                if (owner.Load() != tid) {
                    while (!owner.CmpExchange({}, tid));
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
            Core::Atomic<ThreadAPI::ThreadID> owner;
            Core::AtomicCounter lockCount{0};
        };
    }
}