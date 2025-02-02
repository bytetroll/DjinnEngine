#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class SwapMode {
            eImmediate,
            eMailbox,
            eFIFO,
            eFIFORelaxed
        };

        // Bitset
        DJINN_CORE_BITSET(SwapMode);
    }
}