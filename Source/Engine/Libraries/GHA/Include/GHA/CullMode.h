#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class CullMode {
            eNone = 1,
            eFront = static_cast<int>(eNone) << 1,
            eBack = static_cast<int>(eFront) << 1,
            eFrontBack = static_cast<int>(eBack) << 1
        };

        // Bitset
        DJINN_CORE_BITSET(CullMode);
    }
}