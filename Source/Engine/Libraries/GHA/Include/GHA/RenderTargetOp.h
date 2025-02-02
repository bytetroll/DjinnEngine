#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class RenderTargetLoadOp {
            // No control, may decide on any internal operation
                    eUndefined,

            // Keep previous data
                    eLoad,

            // Clear previous data
                    eClear
        };

        enum class RenderTargetWriteOp {
            // No control, may decide on any internal operation
                    eUndefined,

            // Write to target
                    eWrite,
        };

        // Bitsets
        DJINN_CORE_BITSET(RenderTargetLoadOp);
        DJINN_CORE_BITSET(RenderTargetWriteOp);
    }
}