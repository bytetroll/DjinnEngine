#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class BlendOp : UInt8 {
            eAdd,
            eSub,
            eMin,
            eMax
        };
    }
}