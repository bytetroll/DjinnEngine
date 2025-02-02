#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class TextureUsage {
            eColor = 1,
            eDepthStencil = static_cast<int>(eColor) << 1,
        };

        // Bitset
        DJINN_CORE_BITSET(TextureUsage);
    }
}