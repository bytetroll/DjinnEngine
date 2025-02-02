//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include "Mutex.h"

namespace Djinn {
    namespace Platform {
        template< class T = Mutex >
        class LockGuard {
        public:
            LockGuard( T& mutex ) : mutex( mutex ) {
                mutex.Lock( );
            }

            ~LockGuard( ) {
                mutex.Unlock( );
            }

        private:
            T& mutex;
        };
    }
}