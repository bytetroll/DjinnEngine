#pragma once

#include <Core/String.h>
#include "Platform/LibraryAPI.h"
#include "LibraryHook.h"

namespace Djinn {
    namespace Platform {
        class Library {
        public:
            Library();

            // Load by name
            bool Load(const char *name, const char** outError);

            // Free
            void Free();

            /// Get named address
            /// \tparam T
            /// \param name
            /// \return
            template<class T>
            DJINN_INLINE T GetNamedAddress(const char *name) const  {
                return LibraryAPI::GetNamedAddress<T>(handle, name);
            }

            /// Get hook
            /// \tparam T
            /// \return
            template<typename T>
            DJINN_INLINE LibraryHook<T> GetHook() const {
                auto addr = GetNamedAddress<typename T::Type*>(T::kName);
                auto user = GetNamedAddress<void**>(T::kUserName);
                return LibraryHook<T>(user, addr);
            }

            /// Get name of this library
            /// \return
            const Core::String& GetName() const {
                return name;
            }

        private:
            Core::String name;
            LibraryAPI::Handle handle;
        };
    }
}