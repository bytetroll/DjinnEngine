#pragma once

#include <Core/Types.h>
#include "AllocationSize.h"

namespace Djinn {
    namespace GHA {
        template <class T>
        struct Bounds {
            Bounds() = default;
            Bounds(T lower, T upper) : lower(lower), upper(upper) {

            }

            // Begin
            T lower;

            // End
            T upper;
        };
    }
}