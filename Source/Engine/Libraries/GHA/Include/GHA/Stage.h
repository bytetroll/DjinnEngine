#pragma once

#include <Core/Types.h>
#include <Core/BitSet.h>
#include <Core/Hash.h>

namespace Djinn {
    namespace GHA {
        enum class Stage {
            eVertex = 1,
            eGeometry = static_cast<int>(eVertex) << 1,
            eHull = static_cast<int>(eGeometry) << 1,
            eDomain = static_cast<int>(eHull) << 1,
            ePixel = static_cast<int>(eDomain) << 1,
            eCompute = static_cast<int>(ePixel) << 1,
            eAll = ~static_cast<int>(0),
        };

        // Bitset
        DJINN_CORE_BITSET(Stage);
    }

    // Hash helper
    template<> struct Hasher<GHA::Stage> { USize operator()(GHA::Stage value) const { return Hasher<Int8>{}(static_cast<Int8>(value)); }};
}