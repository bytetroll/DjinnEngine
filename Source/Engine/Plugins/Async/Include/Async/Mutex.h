//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include "Async.h"
#include <Platform/Mutex.h>

namespace Djinn {
    namespace Async {
        class Mutex {
        public:
            void Lock() {
                while (!mutex.TryLock( )) {
                    Yield();
                }
            }

            void Unlock() {
                mutex.Unlock();
            }

        private:
            Platform::Mutex mutex;
        };
    }
}