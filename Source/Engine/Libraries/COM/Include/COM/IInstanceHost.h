#pragma once

#include "Result.h"

namespace Djinn {
    namespace Host {
        class IRegistry;

        void Free(struct null *context);
    }

    namespace COM {
        class IUnknown;

        // Instance worldHost
        // Acts as a spawner / creator for a type
        class IInstanceHost {
        public:
            virtual ~IInstanceHost() {}

            /// Create an instance of a class
            /// \param registry
            /// \param outer
            /// \param out
            /// \return
            virtual Result CreateInstance(Host::IRegistry* registry, IUnknown* outer, IUnknown** out) = 0;

            /// Destroy an instance of a class
            /// \param com
            /// \return
            virtual Result DestroyInstance(IUnknown* com) = 0;
        };
    }
}