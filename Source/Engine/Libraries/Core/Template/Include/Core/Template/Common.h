#pragma once

#include <Core/Size.h>
#include <algorithm>

namespace Djinn {
    namespace Core {
        // Dummy trait
        struct TDummy {
        };

        // Ignore
        template<typename T>
        struct CTIgnore {
            typedef int Type;
        };

        template<typename...>
        using Void = void;

        // Constant
        template<class T, T V>
        struct Value {
            static constexpr T kValue = V;
        };

        // Boolean Aliases
        typedef Value<bool, true> CTTrue;
        typedef Value<bool, false> CTFalse;

        // Select based on condition
        template<bool C, class THEN, class ELSE>
        struct Select {
            typedef THEN Type;
        };
        template<class THEN, class ELSE>
        struct Select<false, THEN, ELSE> {
            typedef ELSE Type;
        };

        // Check if two types are the same
        template<class X, class Y>
        struct IsSame {
            static constexpr bool kValue = false;
        };
        template<class X>
        struct IsSame<X, X> {
            static constexpr bool kValue = true;
        };

        /// Check if two values are the same
        template<auto, auto>
        struct IsSameValue {
            static constexpr bool kValue = true;
        };

        template<auto V>
        struct IsSameValue<V, V> {
            static constexpr bool kValue = false;
        };

        // Check is class is base of class
        template<typename X, typename Y>
        struct IsBaseOf {
            static constexpr bool kValue = __is_base_of(X, Y);
        };

        struct IsStaticCastableImpl {
            template<typename _From, typename _To, typename = decltype(static_cast<_To>(_From()))>
            static CTTrue __test(int);

            template<typename, typename>
            static CTFalse __test(...);
        };

        template<typename From, typename To>
        inline constexpr bool IsCastable = std::is_convertible_v<From, To>;

        // Enable type on condition
        template<bool T, typename R = void>
        struct EnableIf {
        };
        template<typename R>
        struct EnableIf<true, R> {
            typedef R Type;
        };

        // Wrap type
        template<typename T>
        struct Wrapper {
            typedef T Type;
        };

        // Fallback wrapper
        template<class FALLBACK, class X, class Y>
        struct FallbackWrapper {
            using Type = X;
        };

        // Fallback wrapper
        template<class FALLBACK, class Y>
        struct FallbackWrapper<FALLBACK, void, Y> {
            using Type = Y;
        };

        // Remove reference
        template<typename T>
        struct RemoveReference {
            using Type = T;
        };
        template<typename T>
        struct RemoveReference<T &> {
            using Type = T;
        };
        template<typename T>
        struct RemoveReference<T &&> {
            using Type = T;
        };

        // Remove constant reference
        template<typename T>
        struct RemoveConstReference {
            using Type = T;
        };
        template<typename T>
        struct RemoveConstReference<const T> {
            using Type = T;
        };
        template<typename T>
        struct RemoveConstReference<const T &> {
            using Type = T;
        };
        template<typename T>
        struct RemoveConstReference<const T &&> {
            using Type = T;
        };

        namespace Detail {
            template<USize...>
            struct MaxSize;

            template<USize A, USize B>
            struct MaxSize<A, B> {
                static constexpr USize kValue = A > B ? A : B;
            };

            template<USize A, USize B, USize C, USize... X>
            struct MaxSize<A, B, C, X...> {
                static constexpr USize kValue = MaxSize<A, MaxSize<B, C, X...>::kValue>::kValue;
            };

            template<typename T, int BASE, typename... TX>
            struct IndexOf;

            template<typename T, int BASE>
            struct IndexOf<T, BASE> {
                // some error mechanism here
            };

            template<typename T, int BASE, typename... TX>
            struct IndexOf<T, BASE, T, TX...> {
                static constexpr int kValue = BASE;
            };

            template<typename T, int BASE, typename T1, typename... TX>
            struct IndexOf<T, BASE, T1, TX...> {
                static constexpr int kValue = IndexOf<T, BASE + 1, TX...>::kValue;
            };
        }

        /// Get the maximum size
        template<USize... VX>
        static constexpr USize MaxSize = Detail::MaxSize<VX...>::kValue;

        /// Remove outermost reference and const
        template<typename T>
        using RemoveCR = std::remove_const_t<std::remove_reference_t<std::remove_const_t<T>>>;

        /// Get the index of a type within a pack
        template<typename T, typename... TX>
        static constexpr int IndexOf = Detail::IndexOf<T, 0, TX...>::kValue;
    }
}