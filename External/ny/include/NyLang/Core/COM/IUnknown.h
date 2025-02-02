#pragma once

#include <NyLang/Core/Shared.h>
#include "ID.h"

namespace NyLang {
    class Registry;

    class IUnknown {
    public:
        LANG_INTERFACEID("Lang.IUnknown");

        /// Construct
        IUnknown() {

        }

        /// Get super class id
        /// \return
        virtual ClassID GetClassID() = 0;
    };

    /// Cast helper
    template<typename T>
    T* OuterCast(IUnknown*shared) {
        if (shared && shared->GetClassID() == T::kCID) {
            return static_cast<T*>(shared);
        }
        return nullptr;
    }

    /// Cast helper
    template<typename T>
    T* StaticCast(IUnknown*shared) {
        return static_cast<T*>(shared);
    }
}