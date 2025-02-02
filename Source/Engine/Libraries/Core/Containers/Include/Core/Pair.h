#pragma once

#include <Core/Common.h>
#include <Core/Template/Common.h>
#include <type_traits>

namespace Djinn {
    namespace Core {
        template<typename A, typename B>
        struct Pair {
            /// Initialize by value
            /// \param first
            /// \param second
            Pair(const A& first, const B& second) : first(first), second(second) {

            }

            /// Void initialization, not enabled on referenced types
            template<typename DA = A, typename DB = B>
            Pair(typename EnableIf<!(std::is_reference<DA>::value || std::is_reference<DB>::value)>::Type* = nullptr) {

            }

            A first;
            B second;
        };
    }
}