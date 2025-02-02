#pragma once

namespace Djinn {
    // Default comparator
    template<typename T>
    struct Comparator {
        bool operator()(const T& lhs, const T &rhs) const {
            return lhs == rhs;
        }
    };
}