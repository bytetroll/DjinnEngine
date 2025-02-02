#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class QueueType {
            eGraphics = 1,
            eCompute = static_cast<int>(eGraphics) << 1,
            eTransfer = static_cast<int>(eCompute) << 1
        }; DJINN_CORE_BITSET(QueueType);
    }
}