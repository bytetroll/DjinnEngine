#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class SamplerMipMapMode {
            eNearest = 1,
            eLinear = static_cast<int>(eNearest) << 1,
        };

        // Bitset
        DJINN_CORE_BITSET(SamplerMipMapMode);
    }
}