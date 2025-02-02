//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 12/30/2017.
//
#pragma once

namespace Djinn::Core {
    template<typename C, typename T>
    class Member {
    public:
        static constexpr bool kAssignable = std::is_copy_assignable_v<T>;

        Member() = default;
        Member(const Member &) = default;

        /// Make from pointer
        /// \return
        template<T C::*ptr>
        static Member Make(C* frame = nullptr) {
            if constexpr(kAssignable) {
                return Member(frame, Getter < ptr > , Setter < ptr > );
            } else {
                return Member(frame, Getter < ptr > , nullptr);
            }
        }

        /// Stub constructor
        /// \param getter
        /// \param setter
        Member(C* frame, T &(*getter)(C *), T &(*setter)(C *, T &)) {
            stub.frame = frame;
            stub.getter = getter;
            stub.setter = setter;
        }

        /// Is this member assignable?
        /// \return
        bool IsAssignable() const {
            return kAssignable;
        }

        /// Get member value in instance
        /// \param instance
        /// \return
        T &Get(C *instance) const {
            return stub.getter(instance);
        }

        /// Get member value in instance
        /// \param instance
        /// \return
        T &Get() const {
            return stub.getter(stub.frame);
        }

        /// Get member value in instance
        /// \param instance
        /// \return
        T &Get(C &instance) const {
            return stub.getter(&instance);
        }

        /// Set member value in instance
        /// \param instance
        /// \param value
        /// \return
        T &Set(T &value) const {
            DJINN_ASSERT(kAssignable, "Member must be assignable");
            return stub.setter(stub.frame, value);
        }

        /// Set member value in instance
        /// \param instance
        /// \param value
        /// \return
        T &Set(C *instance, T &value) const {
            DJINN_ASSERT(kAssignable, "Member must be assignable");
            return stub.setter(instance, value);
        }

        /// Set member value in instance
        /// \param instance
        /// \param value
        /// \return
        T &Set(C &instance, T &value) const {
            DJINN_ASSERT(kAssignable, "Member must be assignable");
            return stub.setter(&instance, value);
        }

        /// Is this member valid?
        /// \return
        bool IsValid() const {
            return stub.getter != nullptr;
        }

    private:
        struct Stub {
            C* frame = nullptr;
            T &(*getter)(C *) = nullptr;
            T &(*setter)(C *, T &) = nullptr;
        } stub;

        /// Member getter helper
        template<T C::*ptr>
        static T &Getter(C *frame) {
            return frame->*ptr;
        }

        /// Member getter helper
        template<T C::*ptr>
        static T& Setter(C *frame, T &value) {
            (frame->*ptr) = value;
            return frame->*ptr;
        }
    };

    /// Creators
    namespace Detail {
        template<typename>
        struct MemberCreator;

        template<typename T, typename C>
        struct MemberCreator<T C::*> {
            template<T C::*M>
            Member<C, T> Create(void* frame) {
                return Member<C, T>::template Make<M>(frame);
            }
        };
    }

    /// Bind member
    /// Fx. Bindm(Foo::localMember)
#define Bindm(INSTANCE, MEMBER) (Detail::MemberCreator<decltype(& MEMBER)>().Create<& MEMBER>(INSTANCE))

    /// Bind local member
    /// Fx. Bindmm(localMember)
#define Bindmm(MEMBER) (Detail::MemberCreator<decltype(& DJINN_QUALIFIED_THIS::MEMBER)>().Create<& DJINN_QUALIFIED_THIS::MEMBER>(this))
}