#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class TextureFlag {
            // Views to this texture are mutable
            eMutable = 1,

            // Data represents a cube
            eCube = static_cast<int>(eMutable) << 1,

            // The texture can be cleared
            eClearable = static_cast<int>(eCube) << 1,

            // Allows for unordered access such as UAVs
            eUnorderedAccess = static_cast<int>(eClearable) << 1,
        };

        // Bitset
        DJINN_CORE_BITSET(TextureFlag);
    }
}