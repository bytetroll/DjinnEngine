#pragma once

#include <Core/Types.h>

namespace Djinn {
    namespace Platform {
        class User {
        public:
            // Get core count
            static UInt32 GetCoreCount();

            /// Get primary display width
            /// \return
            static UInt32 GetPrimaryDisplayWidth();

            /// Get primary display height
            /// \return
            static UInt32 GetPrimaryDisplayHeight();
        };
    }
}