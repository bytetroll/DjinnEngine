#pragma once

#include <limits>

namespace Djinn {
    namespace Core {
        template <class T, typename LIMITS = std::numeric_limits<T>>
        struct LimitsOf {
            static constexpr T kMax = LIMITS::max();
            static constexpr T kMin = LIMITS::min();
        };
    }
}