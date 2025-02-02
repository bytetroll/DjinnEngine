#pragma once

#include <Core/Template/Limits.h>

namespace Djinn {
    namespace Core {
        template <class T, T MIN = LimitsOf<T>::kMin, T MAX = LimitsOf<T>::kMax>
        struct Range {
            static_assert(MIN <= MAX, "Range minimum must be smaller or equal to the maxiumum");

        public:
            Range(const T& value = T()) : value(value) {
                // ...
            }

            Range(const T&& value) : value(value) {
                // ...
            }

        private:
            T value;
        };
    }
}