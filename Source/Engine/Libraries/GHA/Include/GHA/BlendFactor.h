#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class BlendFactor : UInt8 {
            eZero,
            eOne,
            eSrcColor,
            eOneMinusSrcColor,
            eDstColor,
            eOneMinusDstColor,
            eSrcAlpha,
            eOneMinusSrcAlpha,
            eDstAlpha,
            eOneMinusDstAlpha,
        };
    }
}