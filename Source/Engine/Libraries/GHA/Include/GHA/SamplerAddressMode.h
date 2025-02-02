#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class SamplerAddressMode {
            eRepeat = 1,
            eMirroredRepeat = static_cast<int>(eRepeat) << 1,
            eClampToEdge = static_cast<int>(eMirroredRepeat) << 1,
            eClampToBorder = static_cast<int>(eClampToEdge) << 1,
            eMorroredClampToEdge = static_cast<int>(eClampToBorder) << 1,
        };

        // Bitset
        DJINN_CORE_BITSET(SamplerAddressMode);
    }
}