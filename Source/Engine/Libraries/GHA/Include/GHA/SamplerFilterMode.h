#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class SamplerFilterMode {
            eNearest = 1,
            eLinear = static_cast<int>(eNearest) << 1,
            eCubic = static_cast<int>(eLinear) << 1
        };

        // Bitset
        DJINN_CORE_BITSET(SamplerFilterMode);
    }
}