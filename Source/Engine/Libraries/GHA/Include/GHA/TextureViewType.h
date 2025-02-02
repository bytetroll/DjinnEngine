#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class TextureViewType {
            eUndefined = 0,
            eColor = 1,
            eDepth = static_cast<int>(eColor) << 1,
            eStencil = static_cast<int>(eDepth) << 1,
        };

        // Bitset
        DJINN_CORE_BITSET(TextureViewType);
    }
}