#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class FillMode {
            eFill = 1,
            eLine = static_cast<int>(eFill) << 1,
            ePoint = static_cast<int>(eLine) << 1
        };

        // Bitset
        DJINN_CORE_BITSET(FillMode);
    }
}