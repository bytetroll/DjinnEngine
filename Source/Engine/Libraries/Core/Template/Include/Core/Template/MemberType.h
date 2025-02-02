#pragma once

#include "Common.h"
#include <utility>

namespace Djinn {
    namespace Core {
        template< class >
        struct MemberType;

        template< class C, class T >
        struct MemberType< T C::* > {
            typedef T Type;
        };

        // Function Placement helpers
#define DJINN_SPAWNABLE_PLACEMENT(NO_RETURN) NO_RETURN
#define DJINN_SPAWNABLE_PLACEMENT_RETURN(NO_RETURN, DUMMY_VALUE) NO_RETURN == DUMMY_VALUE

        // Spawn HasFunction helper
        // E.g. DJINN_SPAWNABLE_HASFUNCTION(HasSerialization, operator>>(DummyOStream),
#define DJINN_SPAWNABLE_HASFUNCTION(NAME, FUNCTION_PLACEMENT) \
        template<typename T> \
        struct NAME { \
        private: \
            template<typename U> static auto Pass(int) -> decltype(std::declval<U>(). FUNCTION_PLACEMENT, Core::CTTrue()); \
            template<typename> static Core::CTFalse Pass(...); \
        public: \
            static constexpr bool kValue = Core::IsSame<decltype(Pass<T>(0)), Core::CTTrue>::kValue; \
        };
#define DJINN_HASFUNCTION_ASSERT(TYPE, FUNCTION_PLACEMENT, ERROR_MESSAGE) \
        template <class T = TYPE> struct __FTC_##__COUNTER__ { \
        private: \
            template<typename U> static auto Pass(int) -> decltype(std::declval<U>(). FUNCTION_PLACEMENT, CTTrue()); \
            template<typename> static CTFalse Pass(...); \
        public: \
            static_assert(IsSame<decltype(Pass<T>(0)), CTTrue>::kValue, ERROR_MESSAGE); \
        };

        // Spawn HasMember helper
#define DJINN_SPAWNABLE_HASMEMBER(NAME, MEMBER) \
        template <typename T, typename = int> \
        struct NAME : Djinn::Core::CTFalse { }; \
        template <typename T> \
        struct NAME <T, decltype((void) T::MEMBER, 0)> : Djinn::Core::CTTrue { }; \
        template <typename T> \
        struct NAME <T, decltype((void) typename T::MEMBER(), 0)> : Djinn::Core::CTTrue { };
    }
}