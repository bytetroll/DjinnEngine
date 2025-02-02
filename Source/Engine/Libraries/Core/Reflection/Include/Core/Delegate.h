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

#include <Core/Template/TuplePack.h>
#include <Core/Assert.h>
#include <type_traits>
#include <Core/TypeName.h>
#include <Core/Template/MemberType.h>
#include "DynamicParameterStack.h"

namespace Djinn::Core {
    /// Runtime information for a delegate
    struct DelegateRTTI {
        /// Name of this delegate
        const char *name = nullptr;

        /// Frame name of this delegate
        /// Note: May be null
        const char *frameName = nullptr;

        /// Number of arguments
        UInt8 argumentCount = 0;

        /// Argument names
        char **argumentNames = nullptr;

        /// Argument type names
        char **argumentTypeNames = nullptr;

        /// Return type name
        const char *returnTypeName = nullptr;

        /// Signature type name
        const char *signatureTypeName = nullptr;
    };

    /// Delegate type
    /// \tparam T - Functional type, Fx. float(int, float)
    /// \tparam BUFFER_SIZE - Size of capture buffer, should almost always be default
    template<typename T, int BUFFER_SIZE = 64>
    class Delegate;

    /// Delegate checker
    template<typename>
    struct IsDelegate {
        static constexpr bool kValue = false;
    };

    /// Delegate checker
    template<typename T, int BUFFER_SIZE>
    struct IsDelegate<Delegate<T, BUFFER_SIZE>> {
        static bool const kValue = true;
    };

    /// Default delegate implementation
    template<typename R, typename... A, int BUFFER_SIZE>
    class Delegate<R(A...), BUFFER_SIZE> {
        /// Store
        static constexpr USize kBufferSize = BUFFER_SIZE;

    public:
        /// Static types
        using ReturnType = R;
        using ArgTuple = std::tuple<A...>;

        /// Zero constructor
        Delegate(std::nullptr_t = nullptr) {
            // ...
        }

        /// Copy constructor
        Delegate(const Delegate &other) {
            stub.bufferSize = other.stub.bufferSize;
            stub.object = other.stub.object;
            stub.rtti = other.stub.rtti;
            stub.bufferCopier = other.stub.bufferCopier;
            stub.bufferDeleter = other.stub.bufferDeleter;

            if (other.stub.bufferSize == 0) {
                return;
            }

            // May need custom storage
            if (other.stub.bufferSize > kBufferSize) {
                stub.buffer = new char[stub.bufferSize];
                //Core::Memory::Copy(stub.buffer, other.stub.buffer, stub.bufferSize);
                stub.bufferCopier(stub.buffer, other.stub.buffer);
            } else if (other.stub.buffer != other.stub._buffer) {
                stub.buffer = other.stub.buffer;
            } else {
                stub.buffer = stub._buffer;
                //Core::Memory::Copy(stub.buffer, other.stub.buffer, stub.bufferSize);
                stub.bufferCopier(stub.buffer, other.stub.buffer);
            }
        }

        /// Assignment
        Delegate &operator=(const Delegate &other) {
            if (stub.bufferDeleter) {
                stub.bufferDeleter(stub.buffer);
            }
            if (stub.bufferSize > kBufferSize) {
                delete stub.buffer;
            }

            stub.bufferSize = other.stub.bufferSize;
            stub.object = other.stub.object;
            stub.rtti = other.stub.rtti;
            stub.bufferCopier = other.stub.bufferCopier;
            stub.bufferDeleter = other.stub.bufferDeleter;

            if (other.stub.bufferSize == 0) {
                return *this;
            }

            // May need custom storage
            if (other.stub.bufferSize > kBufferSize) {
                stub.buffer = new char[stub.bufferSize];
                //Core::Memory::Copy(stub.buffer, other.stub.buffer, stub.bufferSize);
                stub.bufferCopier(stub.buffer, other.stub.buffer);
            } else if (other.stub.buffer != other.stub._buffer) {
                stub.buffer = other.stub.buffer;
            } else {
                stub.buffer = stub._buffer;
                //Core::Memory::Copy(stub.buffer, other.stub.buffer, stub.bufferSize);
                stub.bufferCopier(stub.buffer, other.stub.buffer);
            }
            return *this;
        }

        /// Destructor
        ~Delegate() {
            if (stub.bufferDeleter) {
                stub.bufferDeleter(stub.buffer);
            }
            if (stub.bufferSize > kBufferSize) {
                delete stub.buffer;
            }
        }

        /// Initialize from global function
        /// \tparam ptr
        template<R(*ptr)(A...)>
        static Delegate Make() {
            return Delegate(nullptr, GlobalInvoker < ptr > , GlobalRTTI < ptr > );
        }

        /// Initialize from member function
        /// \tparam ptr
        template<typename C, R(C::*ptr)(A...)>
        static Delegate Make(C &frame) {
            return Delegate(static_cast<void *>(&frame), MemberInvoker < C, ptr > , MemberRTTI < C, ptr > );
        }

        /// Initialize from member function
        /// \tparam ptr
        template<typename C, R(C::*ptr)(A...) const>
        static Delegate Make(C &frame) {
            return Delegate(static_cast<void *>(&frame), MemberInvoker < C, ptr > , MemberRTTI < C, ptr > );
        }

        /// Initialize from member function
        /// \tparam ptr
        template<typename C, R(C::*ptr)(A...)>
        static Delegate Make(C *frame) {
            return Delegate(static_cast<void *>(frame), MemberInvoker < C, ptr > , MemberRTTI < C, ptr > );
        }

        /// Initialize from member function
        /// \tparam ptr
        template<typename C, R(C::*ptr)(A...) const>
        static Delegate Make(C *frame) {
            return Delegate(static_cast<void *>(frame), MemberInvoker < C, ptr > , MemberRTTI < C, ptr > );
        }

        /// Create from lambda function
        /// \param instance
        template<
                typename F,
                typename = typename EnableIf<!std::is_function<F>::value && !std::is_same_v<std::decay_t<F>, class DynamicDelegate> && !IsDelegate<F>::kValue>::Type,
                typename = typename EnableIf<std::is_invocable_v<F, A...>>::Type
        >
        Delegate(const F &&instance) : Delegate(instance) {
            // ...
        }

        /// Create from lambda function
        /// \param instance
        template<
                typename F,
                typename = typename EnableIf<!std::is_function<F>::value && !std::is_same_v<std::decay_t<F>, class DynamicDelegate> && !IsDelegate<F>::kValue>::Type,
                typename = typename EnableIf<std::is_invocable_v<F, A...>>::Type
        >
        Delegate(const F &instance) {
            stub.bufferSize = sizeof(F);
            if constexpr(sizeof(F) > kBufferSize) {
                stub.buffer = new char[sizeof(F)];
            }
            //Core::Memory::Copy(stub.buffer, &instance, sizeof(F));
            LambdaBufferCopier<F>(stub.buffer, &instance);
            stub.bufferCopier = LambdaBufferCopier<F>;
            stub.bufferDeleter = LambdaBufferDeleter<F>;
            stub.object = LambdaInvoker<F>;
            stub.rtti = LambdaRTTI<F>;
        }

        /// Stub constructor
        Delegate(void *data, R (*object)(void *, A...), DelegateRTTI(*rtti)(void *user)) {
            stub.bufferSize = sizeof(void *);
            stub.buffer = static_cast<char *>(data);
            stub.object = object;
            stub.rtti = rtti;
        }

        /// Invoke function
        /// \param args
        /// \return
        R operator()(A... args) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            return Invoke(static_cast<A>(args)...);
        }

        /// Invoke function
        /// \param args
        /// \return
        R Invoke(A... args) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            return stub.object(stub.buffer, static_cast<A>(args)...);
        }

        /// Invoke function from a parameter stack
        /// \param stack
        /// \return
        R InvokeStack(const ParameterStack<A...> &stack) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            return InvokeProxy(ToIndexTuple<SafeDecay<A>...>(), static_cast<const typename ParameterStack<A...>::Tuple&>(stack));
        }

        /// Invoke function from a dynamic parameter stack
        /// \param stack
        /// \return
        R InvokeStack(const DynamicParameterStack &stack) const {
            return InvokeStack(stack.Get<A...>());
        }

        /// Get runtime type information of the bound function
        /// \return
        DelegateRTTI GetRTTI() const {
            DJINN_ASSERT(IsValid(), "Getting RTTI of an uninitialized delegate");
            return stub.rtti(nullptr);
        }

        /// Set the rtti callbacks from another delegate
        /// \tparam T
        /// \param other
        template<typename _T, int _BUFFER_SIZE>
        void SetRTTI(const Delegate<_T, _BUFFER_SIZE> &other) {
            stub.rtti = other.stub.rtti;
        }

        /// Is this delegate valid?
        /// \return
        bool IsValid() const {
            return stub.object != nullptr;
        }

        /// Is this delegate valid?
        /// \return
        operator bool() const {
            return IsValid();
        }

        /// Data stub
        struct Stub {
            USize bufferSize = 0;
            char _buffer[kBufferSize] = {};
            char *buffer = _buffer;

            R (*object)(void *, A...) = nullptr;

            void (*bufferCopier)(void *dst, const void *src) = nullptr;

            void (*bufferDeleter)(void *buffer) = nullptr;

            DelegateRTTI (*rtti)(void *user) = nullptr;
        };

        /// Get stub
        /// \return
        const Stub &GetStub() const {
            return stub;
        }

        /// Get stub
        /// \return
        Stub &GetStub() {
            return stub;
        }

    private:
        mutable Stub stub;

        /// Invocation proxy
        /// \return
        template<unsigned... I, typename T>
        R InvokeProxy(IndexTuple<I...>, T &&t) const {
            return InvokeProxy(std::get<I>(t)...);
        }

        /// Invocation proxy
        /// \return
        R InvokeProxy(SafeDecay<A>... args) const {
            return stub.object(stub.buffer, static_cast<A>(args)...);
        }

        template<typename T0, typename T1, typename... TX>
        static void PopulateRTTIArguments(DelegateRTTI &rtti, UInt8 index) {
            PopulateRTTIArguments < T0 > (rtti, index);
            PopulateRTTIArguments < T1, TX...>(rtti, index + 1);
        }

        template<typename T0>
        static void PopulateRTTIArguments(DelegateRTTI &rtti, UInt8 index) {
            rtti.argumentTypeNames[index] = GetTypeName<T0>();
        }

        /// Global invocation helper
        template<R(*ptr)(A...)>
        static R GlobalInvoker(void *, A... args) {
            if constexpr(!IsSame<R, void>::kValue) {
                return ptr(static_cast<A>(args)...);
            } else {
                ptr(static_cast<A>(args)...);
            }
        }

        /// Global rtti helper
        template<R(*ptr)(A...)>
        static DelegateRTTI GlobalRTTI(void *user) {
            // Construct static rtti
            static DelegateRTTI srtti = [] {
                DelegateRTTI rtti = {};
                rtti.name = GetTypeName<decltype(ptr), ptr>();
                rtti.frameName = nullptr;
                rtti.argumentCount = sizeof...(A);
                rtti.returnTypeName = GetTypeName<R>();
                rtti.signatureTypeName = GetTypeName<R(A...)>();
                if constexpr(sizeof...(A) > 0) {
                    rtti.argumentNames = new char *[rtti.argumentCount];
                    rtti.argumentTypeNames = new char *[rtti.argumentCount];
                    for (UInt8 i = 0; i < rtti.argumentCount; i++) {
                        rtti.argumentNames[i] = new char(i);
                    }
                    PopulateRTTIArguments<A...>(rtti, 0);
                }
                return rtti;
            }();

            // ...
            return srtti;
        }

        /// Member invocation helper
        template<typename C, R(C::*ptr)(A...)>
        static R MemberInvoker(void *super, A... args) {
            if constexpr(!IsSame<R, void>::kValue) {
                return (static_cast<C *>(super)->*ptr)(static_cast<A>(args)...);
            } else {
                (static_cast<C *>(super)->*ptr)(static_cast<A>(args)...);
            }
        }

        /// Member invocation helper
        template<typename C, R(C::*ptr)(A...) const>
        static R MemberInvoker(void *super, A... args) {
            if constexpr(!IsSame<R, void>::kValue) {
                return (static_cast<C *>(super)->*ptr)(static_cast<A>(args)...);
            } else {
                (static_cast<C *>(super)->*ptr)(static_cast<A>(args)...);
            }
        }

        /// Member rtti helper
        template<typename C, R(C::*ptr)(A...)>
        static DelegateRTTI MemberRTTI(void *user) {
            // Construct static rtti
            static DelegateRTTI srtti = [] {
                DelegateRTTI rtti = {};
                rtti.name = GetTypeName<decltype(ptr), ptr>();
                rtti.frameName = GetTypeName<C>();
                rtti.argumentCount = sizeof...(A);
                rtti.returnTypeName = GetTypeName<R>();
                rtti.signatureTypeName = GetTypeName<R(A...)>();
                if constexpr(sizeof...(A) > 0) {
                    rtti.argumentNames = new char *[rtti.argumentCount];
                    rtti.argumentTypeNames = new char *[rtti.argumentCount];
                    for (UInt8 i = 0; i < rtti.argumentCount; i++) {
                        rtti.argumentNames[i] = new char(i);
                    }
                    PopulateRTTIArguments<A...>(rtti, 0);
                }
                return rtti;
            }();

            // ...
            return srtti;
        }

        /// Member rtti helper
        template<typename C, R(C::*ptr)(A...) const>
        static DelegateRTTI MemberRTTI(void *user) {
            // Construct static rtti
            static DelegateRTTI srtti = [] {
                DelegateRTTI rtti = {};
                rtti.name = GetTypeName<decltype(ptr), ptr>();
                rtti.frameName = GetTypeName<C>();
                rtti.argumentCount = sizeof...(A);
                rtti.returnTypeName = GetTypeName<R>();
                rtti.signatureTypeName = GetTypeName<R(A...)>();
                if constexpr(sizeof...(A) > 0) {
                    rtti.argumentNames = new char *[rtti.argumentCount];
                    rtti.argumentTypeNames = new char *[rtti.argumentCount];
                    for (UInt8 i = 0; i < rtti.argumentCount; i++) {
                        rtti.argumentNames[i] = new char(i);
                    }
                    PopulateRTTIArguments<A...>(rtti, 0);
                }
                return rtti;
            }();

            // ...
            return srtti;
        }

        /// Lambda invocation helper
        template<typename F>
        static R LambdaInvoker(void *buffer, A... args) {
            if constexpr(!IsSame<R, void>::kValue) {
                return static_cast<F *>(buffer)->operator()(static_cast<A>(args)...);
            } else {
                static_cast<F *>(buffer)->operator()(static_cast<A>(args)...);
            }
        }

        /// Lambda rtti helper
        template<typename F>
        static DelegateRTTI LambdaRTTI(void *user) {
            // Construct static rtti
            static DelegateRTTI srtti = [] {
                DelegateRTTI rtti = {};
                rtti.name = GetTypeName<F>();
                rtti.frameName = nullptr;
                rtti.argumentCount = sizeof...(A);
                rtti.returnTypeName = GetTypeName<R>();
                rtti.signatureTypeName = GetTypeName<R(A...)>();
                if constexpr(sizeof...(A) > 0) {
                    rtti.argumentNames = new char *[rtti.argumentCount];
                    rtti.argumentTypeNames = new char *[rtti.argumentCount];
                    for (UInt8 i = 0; i < rtti.argumentCount; i++) {
                        rtti.argumentNames[i] = new char(i);
                    }
                    PopulateRTTIArguments<A...>(rtti, 0);
                }
                return rtti;
            }();

            // ...
            return srtti;
        }

        /// Lambda buffer deleter
        template<typename F>
        static void LambdaBufferDeleter(void *buffer) {
            reinterpret_cast<F *>(buffer)->~F();
        }

        /// Lambda buffer copier
        template<typename F>
        static void LambdaBufferCopier(void *dest, const void *src) {
            new(dest) F(*reinterpret_cast<const F *>(src));
        }
    };

    /// A shorthand with a zero buffer delegate
    template<typename T>
    using FreeDelegate = Delegate<T, 0>;

    /// A shorthand with no arguments or return type
    using VoidDelegate = Delegate<void()>;

    /// Constructors
    namespace Detail {
        template<typename>
        struct DelegateCreator;

        template<typename C, typename R, typename... A>
        struct DelegateCreator<R(C::*)(A...)> {
            template<R(C::*F)(A...)>
            Delegate<R(A...)> Create(C *instance) {
                return Delegate<R(A...)>::template Make<C, F>(instance);
            }

            template<R(C::*F)(A...)>
            Delegate<R(A...)> Create(C &instance) {
                return Delegate<R(A...)>::template Make<C, F>(instance);
            }
        };

        template<typename C, typename R, typename... A>
        struct DelegateCreator<R(C::*)(A...) const> {
            template<R(C::*F)(A...) const>
            Delegate<R(A...)> Create(C *instance) {
                return Delegate<R(A...)>::template Make<C, F>(instance);
            }

            template<R(C::*F)(A...) const>
            Delegate<R(A...)> Create(C &instance) {
                return Delegate<R(A...)>::template Make<C, F>(instance);
            }
        };

        template<typename R, typename... A>
        struct DelegateCreator<R(*)(A...)> {
            template<R(*F)(A...)>
            Delegate<R(A...)> Create() {
                return Delegate<R(A...)>::template Make<F>();
            }
        };
    }

    /// Bind member function
    /// Fx. Bindf(objRef, Obj::Function), Bindf(&objRef, Obj::Function)
#define Bindf(INSTANCE, FUNCTION) (Djinn::Core::Detail::DelegateCreator<decltype(& FUNCTION)>().template Create<& FUNCTION>(INSTANCE))

    /// Bind member function shorthand
    /// Fx. Bindff(Function)
#define Bindff(FUNCTION) Bindf(this, DJINN_QUALIFIED_THIS::FUNCTION)

    /// Bind global / static function
    /// Fx. Bindf(Obj::StaticFunction)
    /// Fx. Bindf(GlobalFunction)
#define Bindg(FUNCTION /*    */) (Djinn::Core::Detail::DelegateCreator<decltype(& FUNCTION)>().template Create<& FUNCTION>())
}