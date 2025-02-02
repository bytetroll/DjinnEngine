#pragma once

#include <Core/Types.h>
#include <Core/Assert.h>
#include <Core/Math.h>

namespace Djinn {
    namespace GHA {
        struct AllocationSize {
            AllocationSize(UInt64 width = 0) : width(width) {
#ifdef DJINN_DEBUG
                //DJINN_ASSERT(width % 16 == 0, "Allocation size width must be divisible by 16");
#else
                // Ensure safe width
                // He will pay for his mistakes, but not today... (Tomorrow)
                //width = Math::SnapUpper(width, (UInt64)16);
#endif
            }

            // ...
            UInt64 width;
        };
    }
}