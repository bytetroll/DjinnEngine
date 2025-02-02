//
// Created by dev on 9/12/17.
//

#pragma once

#include <Core/Common.h>

namespace Djinn::Core {
    /// Assign a ref with a value and return old value
    /// \param var
    /// \param value
    /// \return
    template <typename T0, typename T1 = T0>
    DJINN_INLINE T0 Exchange(T0& var, T1&& value) {
        T0 copy = var;
        var = value;
        return copy;
    }
}