#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class TextureType {
            e1D = 1,
            e2D = static_cast<int>(e1D) << 1,
            e3D = static_cast<int>(e2D) << 1,
            eCube = static_cast<int>(e3D) << 1,
        };

        // Bitset
        DJINN_CORE_BITSET(TextureType);
    }
}