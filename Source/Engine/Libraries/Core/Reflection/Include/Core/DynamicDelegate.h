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

#include <Core/Size.h>
#include <Core/Assert.h>
#include <Core/Template/Lambda.h>
#include <Core/TypeCommon.h>
#include <Core/CompileHash.h>
#include "Delegate.h"

namespace Djinn::Core {
    namespace Detail {
        template<typename T>
        struct DynamicDelegateArgSig {
            using Type = T;
        };

        template<typename T>
        struct DynamicDelegateArgSig<const T> {
            using Type = T;
        };

        template<typename T>
        struct DynamicDelegateArgSig<const T &> {
            using Type = T &;
        };

        template<typename T>
        struct DynamicDelegateArgSig<const T &&> {
            using Type = T &&;
        };
    }

    /// Dynamic delegate argument signature, const correctness is not needed
    template<typename T>
    using DynamicDelegateArgSig = typename Detail::DynamicDelegateArgSig<T>::Type;

    template<typename...>
    struct DynamicDelegateLambdaCreator;

    template<typename R, typename... A>
    struct DynamicDelegateLambdaCreator<R, std::tuple<A...>> {
        using Type = Delegate<R(A...)>;
    };

    class DynamicDelegate {
        /// In case of an abnormal lambda storage the delegate should
        static constexpr USize kMaxDelegateSize = sizeof(Delegate<int(int)>) + 32;

    public:
        DynamicDelegate() = default;

        /// Copier
        /// \param other
        DynamicDelegate(const DynamicDelegate &other) {
            stub = other.stub;
            data.buffer = data._buffer;

            // Invoke copier
            if (other.IsValid()) {
                other.stub.delegateCopier(data.buffer, other.data.buffer);
            }
        }

        ~DynamicDelegate() {
            if (IsValid()) {
                // Invoke self deleter
                stub.delegateDeleter(data.buffer);
            }
        }

        /// Construct from delegate, also compatible with Bindf/Bindg
        /// \tparam F
        /// \param delegate
        template<typename R, typename... A>
        DynamicDelegate(const Delegate<R(A...)> &delegate) {
            static_assert(sizeof(delegate) <= kMaxDelegateSize, "Unexpected delegate size");

            // Prepare stubs
            stub.signatureHash = GetTypeNameCRC64<R(A...)>();
            stub.verificationHash = GetTypeNameCRC64<R(DynamicDelegateArgSig<A>...)>();
            stub.delegateDeleter = DelegateDeleter<R, A...>;
            stub.delegateCopier = DelegateCopier<R, A...>;
            stub.dynamicStackInvoker = reinterpret_cast<decltype(stub.dynamicStackInvoker)>(DynamicStackInvoker<R, A...>);
            stub.delegateRTTI = delegate.GetStub().rtti;

            // Fill buffer
            new(data.buffer) Delegate<R(A...)>(delegate);
        }

        DynamicDelegate &operator=(const DynamicDelegate &other) {
            if (IsValid()) {
                // Invoke self deleter
                stub.delegateDeleter(data.buffer);
            }

            // Copy foreign stub and properties
            stub = other.stub;

            // Invoke copier
            if (other.IsValid()) {
                stub.delegateCopier(data.buffer, other.data.buffer);
            }
            return *this;
        }

        /// Construct from delegate, also compatible with Bindf/Bindg
        /// \tparam F
        /// \param delegate
        template<typename R, typename... A>
        DynamicDelegate(Delegate<R(A...)> &&delegate) : DynamicDelegate(delegate) {
            // ...
        }

        /// Create from lambda function
        /// \param instance
        template<
                typename F,
                typename = typename EnableIf<!std::is_function<F>::value && !std::is_same_v<std::decay_t<F>, class DynamicDelegate> && !IsDelegate<F>::kValue>::Type,
                typename TRAITS = LambdaFunctionTraits<F>
        >
        DynamicDelegate(const F &&instance) : DynamicDelegate(typename DynamicDelegateLambdaCreator<typename TRAITS::ResultType, typename TRAITS::ArgTypes>::Type(instance)) {
            // ...
        }

        /// Create from lambda function
        /// \param instance
        template<
                typename F,
                typename = typename EnableIf<!std::is_function<F>::value && !std::is_same_v<std::decay_t<F>, class DynamicDelegate> && !IsDelegate<F>::kValue>::Type,
                typename TRAITS = LambdaFunctionTraits<F>
        >
        DynamicDelegate(const F &instance) : DynamicDelegate(typename DynamicDelegateLambdaCreator<typename TRAITS::ResultType, typename TRAITS::ArgTypes>::Type(instance)) {
            // ...
        }

        /// Invoke function
        /// \param a
        template<typename... A>
        void operator()(A ... a) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            VerifySignature<void, A...>();
            reinterpret_cast<const Delegate<void(A...)> *>(data.buffer)->Invoke(static_cast<A>(a)...);
        }

        /// Attempt to convert this dynamic delegate to a typed delegate
        /// \return
        template<typename T>
        Delegate<T> As() const {
            return AsImpl(static_cast<T *>(nullptr));
        }

        /// Attempt to convert this dynamic delegate to a typed delegate
        /// \return
        template<typename T>
        T AsT() const {
            return AsImplT(static_cast<T *>(nullptr));
        }

        /// Invoke function
        /// \param a
        /// \return
        template<typename... A>
        void Invoke(A ... a) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            VerifySignature<void, A...>();
            reinterpret_cast<const Delegate<void(A...)> *>(data.buffer)->Invoke(static_cast<A>(a)...);
        }

        /// Invoke function
        /// Do not verify the signature, this is useful for function reinterpretation
        /// \param a
        /// \return
        template<typename... A>
        void InvokeUnsafe(A ... a) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            reinterpret_cast<const Delegate<void(A...)> *>(data.buffer)->Invoke(static_cast<A>(a)...);
        }

        /// Invoke function with return type
        /// \param a
        /// \return
        template<typename R, typename... A>
        R Invoke(A ... a) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            VerifySignature<R, A...>();
            return reinterpret_cast<const Delegate<R(A...)> *>(data.buffer)->Invoke(static_cast<A>(a)...);
        }

        /// Invoke function with return type
        /// Do not verify the signature, this is useful for function reinterpretation
        /// \param a
        /// \return
        template<typename R, typename... A>
        R InvokeUnsafe(A ... a) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            return reinterpret_cast<const Delegate<R(A...)> *>(data.buffer)->Invoke(static_cast<A>(a)...);
        }

        /// Invoke function from parameter stack
        /// \param stack
        /// \return
        template<typename... A>
        void InvokeStack(const ParameterStack<A...> &stack) {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            VerifySignature<void, A...>();
            reinterpret_cast<const Delegate<void(A...)> *>(data.buffer)->InvokeStack(stack);
        }

        /// Invoke function from parameter stack
        /// \param stack
        /// \return
        template<typename R, typename... A>
        R InvokeStack(const ParameterStack<A...> &stack) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            VerifySignature<R, A...>();
            return reinterpret_cast<const Delegate<R(A...)> *>(data.buffer)->InvokeStack(stack);
        }

        /// Invoke function from parameter stack
        /// \param stack
        /// \return
        void InvokeStack(const DynamicParameterStack &stack) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            stub.dynamicStackInvoker(data.buffer, stack);
        }

        /// Invoke function from parameter stack
        /// \param stack
        /// \return
        template<typename R>
        R InvokeStack(const DynamicParameterStack &stack) const {
            DJINN_ASSERT(IsValid(), "Invoking an uninitialized delegate");
            return reinterpret_cast<R (*)(const void *, const DynamicParameterStack &)>(stub.dynamicStackInvoker)(data.buffer, stack);
        }

        /// Is this dynamic delegate valid?
        /// \return
        bool IsValid() const {
            return stub.dynamicStackInvoker != nullptr;
        }

        /// Get the signature hash of the assigned delegate
        /// \return
        USize GetSignatureHash() const {
            return stub.signatureHash;
        }

        /// Get runtime type information of the bound function
        /// \return
        DelegateRTTI GetRTTI() const {
            return stub.delegateRTTI(stub.rttiUser);
        }

        /// Set the rtti callbacks from another delegate
        /// \tparam T
        /// \param other
        void SetRTTI(const DynamicDelegate &other) {
            stub.delegateRTTI = other.stub.delegateRTTI;
        }

        /// Data stub
        struct Stub {
            void (*delegateDeleter)(const void *buffer) = nullptr;
            void (*delegateCopier)(void *dest, const void *source) = nullptr;
            void (*dynamicStackInvoker)(const void *buffer, const DynamicParameterStack &) = nullptr;
            DelegateRTTI (*delegateRTTI)(void *user) = nullptr;
            void *rttiUser = nullptr;
            USize signatureHash = 0;
            USize verificationHash = 0;
        };

        /// Data
        struct Data {
            char _buffer[kMaxDelegateSize] = {};
            char *buffer = _buffer;
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

        /// Get data
        /// \return
        const Data &GetData() const {
            return data;
        }

        /// Get stub
        /// \return
        Data &GetData() {
            return data;
        }

    private:
        Stub stub;
        Data data;

        /// Helper
        template<typename R, typename... A>
        Delegate<R(A...)> AsImplT(Delegate<R(A...)> * = nullptr) const {
            return AsImpl(static_cast<R(*)(A...)>(nullptr));
        }

        /// Helper
        template<typename R, typename... A>
        Delegate<R(A...)> AsImpl(R(*)(A...) = nullptr) const {
            if (!IsValid() || !IsSignature<R, A...>()) {
                return nullptr;
            }
            return *reinterpret_cast<const Delegate<void(A...)> *>(data.buffer);
        }

        /// Verify the signature and throw a detailed exception message
        /// \tparam R
        /// \tparam A
        template<typename R, typename... A>
        void VerifySignature() const {
#ifdef DJINN_DEBUG
            if (GetTypeNameCRC64<R(DynamicDelegateArgSig<A>...)>() != stub.verificationHash) {
                if (stub.delegateRTTI) {
                    auto rtti = stub.delegateRTTI(stub.rttiUser);
                    DJINN_ASSERT(false, "Signature mismatch (const insensitive), invoked with signature ", GetStaticTypeName<R(A...)>(), " but is actually ", rtti.signatureTypeName);
                } else {
                    DJINN_ASSERT(false, "Signature mismatch (RTTI not available for a detailed exception message)");
                }
            }
#endif
        }

        /// Verify the signature and throw a detailed exception message
        /// \tparam R
        /// \tparam A
        template<typename R, typename... A>
        bool IsSignature() const {
            return GetTypeNameCRC64<R(DynamicDelegateArgSig<A>...)>() == stub.verificationHash;
        }

        /// Delegate deleters, invokes the deconstruction
        /// \tparam R
        /// \tparam A
        /// \param buffer
        template<typename R, typename... A>
        static void DelegateDeleter(const void *buffer) {
            reinterpret_cast<const Delegate<R(A...)> *>(buffer)->~Delegate();
        }

        /// Delegate copier, copies a delegate of same type to another buffer
        /// \param dest
        /// \param source
        template<typename R, typename... A>
        static void DelegateCopier(void *dest, const void *source) {
            new(dest) Delegate<R(A...)>(*reinterpret_cast<const Delegate<R(A...)> *>(source));
        }

        /// Dynamic stack invocation stub
        /// \param buffer
        /// \param stack
        /// \return
        template<typename R, typename... A>
        static R DynamicStackInvoker(const void *buffer, const DynamicParameterStack &stack) {
            return reinterpret_cast<const Delegate<R(A...)> *>(buffer)->InvokeStack(stack);
        }
    };
}