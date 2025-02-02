#pragma once

#include <Core/Common.h>
#include <Core/Size.h>
#include <cstring>

namespace Djinn {
    namespace Core {
        class Memory {
            DJINN_STATICCLASS(Memory);

        public:
            // Copy memory from source to destination
            static void Copy(void *dest, const void *source, USize size) {
                std::memcpy(dest, source, size);
            }

            // Zero memory
            static void Zero(void *dest, USize size) {
                std::memset(dest, 0, size);
            }

            /// Perform a bit cast
            /// \param source
            /// \return
            template <class T, class U>
            static T BitCast(const U &source) {
                T dest;
                std::memcpy(&dest, &source, sizeof(dest));
                return dest;
            }
        };
    }
}