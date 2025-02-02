#pragma once

#include <memory.h>
#include <Core/Common.h>

namespace Djinn {
    namespace Platform {
        namespace LibraryAPI {
            // Uniform handle
            using Handle = void*;

            // Invalid handle
            static constexpr Handle kInvalid = nullptr;

            /// Get current module handle
            /// \return
            extern Handle GetCurrent();

            /// Load a library by name
            /// \param name
            /// \return
            extern Handle Load(const char* name);

            /// Get last error message
            /// \return
            extern const char* LastError();

            /// Free a loaded library
            /// \param h
            extern void Free(const Handle& h);

            /// Get a named address
            /// \param h
            /// \param name
            /// \return
            extern void* GetNamedAddress(const Handle& h, const char* name);

            /// Helper
            template <class T>
            DJINN_INLINE T GetNamedAddress(const Handle& h, const char* name) {
                return reinterpret_cast<T>(GetNamedAddress(h, name));
            }
        }
    }
}