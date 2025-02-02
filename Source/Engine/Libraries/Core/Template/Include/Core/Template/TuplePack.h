#pragma once

#include <tuple>
#include "Common.h"

namespace Djinn {
    namespace Core {
        /// Safe decay
        /// Does not decay types which are not copy constructible
        template<typename T>
        using SafeDecay = typename Select<std::is_copy_constructible_v<std::decay_t<T>>, std::decay_t<T>, T>::Type;

        // Standard parameter stack
        template<typename... A>
        struct ParameterStack : public std::tuple<SafeDecay<A>...> {
            using Tuple = std::tuple<SafeDecay<A>...>;

            ParameterStack() : std::tuple<SafeDecay<A>...>() {

            }

            ParameterStack(const A &... args) : std::tuple<SafeDecay<A>...>(args...) {

            }
        };

        // Standard parameter stack
        template<>
        struct ParameterStack<> : public std::tuple<> {
            using Tuple = std::tuple<>;

            ParameterStack() : std::tuple<>() {

            }
        };

        // Pack argument into stack
        template<typename ... A>
        ParameterStack<A...> Pack(A... args) {
            return {args...};
        }

        // Tuple index
        template<unsigned... Indices>
        struct IndexTuple {
            template<unsigned N> using append = IndexTuple<Indices..., N>;
        };

        // Tuple index helper
        template<unsigned Size>
        struct MakeIndexTuple {
            typedef typename MakeIndexTuple<Size - 1>::type::template append<Size - 1> type;
        };

        // Zero tuple index
        template<>
        struct MakeIndexTuple<0u> {
            typedef IndexTuple<> type;
        };

        // To index tuple
        template<typename... Types>
        using ToIndexTuple = typename MakeIndexTuple<sizeof...(Types)>::type;
    }
}