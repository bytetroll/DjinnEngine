#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class SampleMode {
            e1 = 1,
            e2 = static_cast<int>(e1) << 1,
            e4 = static_cast<int>(e2) << 1,
            e8 = static_cast<int>(e4) << 1,
            e16 = static_cast<int>(e8) << 1,
            e32 = static_cast<int>(e16) << 1,
            e64 = static_cast<int>(e32) << 1
        };

        // Bitset
        DJINN_CORE_BITSET(SampleMode);
    }
}