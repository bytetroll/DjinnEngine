#pragma once

#include <Core/Types.h>
#include <tuple>
#include "MemberType.h"
#include "TuplePack.h"

namespace Djinn {
    namespace Core {
        template<typename T>
        struct is_callable_impl {
        private:
            typedef char(&yes)[1];
            typedef char(&no)[2];

            struct Fallback { void operator()(); };
            struct Derived : T, Fallback { };

            template<typename U, U> struct Check;

            template<typename>
            static yes test(...);

            template<typename C>
            static no test(Check<void (Fallback::*)(), &C::operator()>*);

        public:
            static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
        };

        template<typename T>
        struct is_callable
                : std::conditional<
                        std::is_class<T>::value,
                        is_callable_impl<T>,
                        std::false_type
                >::type
        { };

        template<typename T>
        struct LambdaFunctionTraits : public LambdaFunctionTraits<decltype(&T::operator())> {
        };

        template<typename ClassType, typename ReturnType, typename... Args>
        struct LambdaFunctionTraits<ReturnType(ClassType::*)(Args...) const> {
            static constexpr UInt32 kArgCount = sizeof...(Args);

            typedef ReturnType ResultType;

            using ArgTypes = std::tuple<Args...>;

            using Stack = ParameterStack<Args...>;

            template<size_t i>
            struct Arg {
                using Type = typename std::tuple_element<i, std::tuple<Args...>>::type;
            };
        };

        // Lambda Parameter Stack Extractor
        template<typename F>
        using AutoLambdaParameterStack = typename LambdaFunctionTraits<F>::ArgTypes;
    }
}