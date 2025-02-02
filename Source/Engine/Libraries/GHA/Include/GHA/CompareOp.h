#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class CompareOp {
            eNever = 1,
            eLess = static_cast<int>(eNever) << 1,
            eEqual = static_cast<int>(eLess) << 1,
            eLessOrEqual = static_cast<int>(eEqual) << 1,
            eGreater = static_cast<int>(eLessOrEqual) << 1,
            eNotEqual = static_cast<int>(eGreater) << 1,
            eGreaterOrEqual = static_cast<int>(eNotEqual) << 1,
            eAlways = static_cast<int>(eGreaterOrEqual) << 1
        };

        // Bitset
        DJINN_CORE_BITSET(CompareOp);
    }
}