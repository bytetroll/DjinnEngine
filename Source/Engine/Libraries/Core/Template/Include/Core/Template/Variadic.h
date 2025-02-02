#pragma once

#include <type_traits>
#include <tuple>
#include "Common.h"

namespace Djinn {
    namespace Core {
        template <typename ...A>
        struct VariadicCompare : CTFalse { };

        template <typename A1, typename ...Aother, typename B1, typename ...Bother>
        struct VariadicCompare<std::tuple<A1, Aother...>, std::tuple<B1, Bother...>>
        {
            static const bool kValue = std::is_same<A1, B1>::value && VariadicCompare<std::tuple<Aother...>, std::tuple<Bother...>>::kValue;
        };

        template <typename ...B>
        struct VariadicCompare<std::tuple<>, std::tuple<B...>> : std::true_type { };

        template<typename...> struct VariadicContains;

        template<typename T, typename T0>
        struct VariadicContains<T, T0> {
            static constexpr bool kValue = IsSame<T, T0>::kValue;
        };

        template<typename T, typename T0, typename T1, typename... TX>
        struct VariadicContains<T, T0, T1, TX...> {
            static constexpr bool kValue = VariadicContains<T, T0>::kValue || VariadicContains<T, T1, TX...>::kValue;
        };
    }
}