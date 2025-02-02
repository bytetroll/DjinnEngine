#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        namespace PipelineScheduler {
            enum class DynamicState {
                eScissor = 1,
                eViewport = static_cast<int>(eScissor) << 1
            };

            // Bitset
            DJINN_CORE_BITSET(DynamicState);
        }
    }
}