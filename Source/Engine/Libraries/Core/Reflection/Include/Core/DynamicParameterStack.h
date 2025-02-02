//
// Created by Dev on 12/30/2017.
//
#pragma once

#include <Core/Size.h>
#include <Core/Template/TuplePack.h>
#include <Core/Memory.h>
#include <Core/CompileHash.h>

namespace Djinn::Core {
    namespace Detail {
        template<typename T>
        struct DynamicParameterStackArgSig {
            using Type = T;
        };

        template<typename T>
        struct DynamicParameterStackArgSig<const T> {
            using Type = T;
        };

        template<typename T>
        struct DynamicParameterStackArgSig<const T &> {
            using Type = T &;
        };

        template<typename T>
        struct DynamicParameterStackArgSig<const T &&> {
            using Type = T &&;
        };
    }

    /// Dynamic delegate argument signature, const correctness is not needed
    template<typename T>
    using DynamicParameterStackArgSig = typename Detail::DynamicParameterStackArgSig<T>::Type;

    /// Runtime information for a parameter stack
    struct ParameterStackRTTI {
        /// Number of paramters
        uint32_t parameterCount;

        /// Types of each parameter
        const char** parameterTypes;

        /// Entire signature of the parameter pack
        const char* signatureTypeName;
    };

    class DynamicParameterStack {
        /// TODO: Mess around with the default storage size
        static constexpr USize kDefaultStackSize = 196;

    public:
        DynamicParameterStack() = default;

        DynamicParameterStack(const DynamicParameterStack & other) : stub(other.stub), size(other.size), isValid(other.isValid) {
            // May need custom storage
            if (size > kDefaultStackSize) {
                buffer = new char[size];
            }

            // Invoke copier
            if (other.IsValid()) {
                stub.packCopier(buffer, other.buffer);
            }
        }

        /// Deconstruct
        ~DynamicParameterStack() {
            if (IsValid()) {
                // Invoke self deleter
                stub.packDeleter(buffer);

                // Buffer may need manual deallocation
                if (size > kDefaultStackSize) {
                    delete buffer;
                }
            }
        }

        DynamicParameterStack& operator=(const DynamicParameterStack& other) {
            if (IsValid()) {
                // Invoke self deleter
                stub.packDeleter(buffer);
            }

            // Copy foreign stub and properties
            stub = other.stub;
            isValid = other.isValid;
            size = other.size;

            // May need custom storage
            buffer = _buffer;
            if (size > kDefaultStackSize) {
                buffer = new char[size];
            }

            // Invoke copier
            if (other.IsValid()) {
                stub.packCopier(buffer, other.buffer);
            }
            return *this;
        }

        /// Construct from arguments
        /// \param a
        template<typename... A>
        DynamicParameterStack(const A &... a) : DynamicParameterStack(ParameterStack<A...>(a...)) {
            // ...
        }

        /// Construct from stack
        /// \param stack
        template<typename... A>
        DynamicParameterStack(const ParameterStack<A...> &stack) : isValid(true) {
            size = sizeof(ParameterStack<A...>);

            // Prepare stub
            stub.verificationHash = GetTypeNameCRC64<void(DynamicParameterStackArgSig<A>...)>();
            stub.packCopier = PackCopier<A...>;
            stub.packDeleter = PackDeleter<A...>;
            stub.parameterRTTI = ParameterRTTI<A...>;

            // Fill buffer
            if constexpr (sizeof(ParameterStack<A...>) > kDefaultStackSize) {
                buffer = static_cast<char*>(new ParameterStack<A...>(stack));
            } else {
                new (buffer) ParameterStack<A...>(ParameterStack<A...>(stack));
            }
        }

        /// Verify the signature
        /// \tparam R
        /// \tparam A
        template<typename... A>
        void VerifySignature() const {
#ifdef DJINN_DEBUG
            if (GetTypeNameCRC64<void(DynamicParameterStackArgSig<A>...)>() != stub.verificationHash) {
                if (stub.parameterRTTI) {
                    auto rtti = stub.parameterRTTI();
                    DJINN_ASSERT(false, "Signature mismatch (const insensitive), queried with signature ", GetStaticPackTypeName<A...>(), " but is actually ", rtti.signatureTypeName);
                } else {
                    DJINN_ASSERT(false, "Signature mismatch (RTTI not available for a detailed exception message)");
                }
            }
#endif
        }

        /// Get parameter stack
        /// \return
        template<typename... A>
        ParameterStack<A...> &Get() {
            VerifySignature<A...>();
            return *reinterpret_cast<ParameterStack<A...> *>(buffer);
        }

        /// Get parameter stack
        /// \return
        template<typename... A>
        const ParameterStack<A...> &Get() const {
            VerifySignature<A...>();
            return *reinterpret_cast<const ParameterStack<A...> *>(buffer);
        }

        /// Get the runtime type information
        /// \return
        ParameterStackRTTI GetRTTI() const {
            return stub.parameterRTTI();
        }

        /// Is this parameter stack valid?
        /// \return
        bool IsValid() const {
            return isValid;
        }

    private:
        struct Stub {
            void (*packDeleter)(const void *buffer) = nullptr;
            void (*packCopier)(void *dest, const void *source) = nullptr;
            ParameterStackRTTI (*parameterRTTI)() = nullptr;
            uint64_t verificationHash;
        } stub;

        /// Pack deleter, invokes the deconstructor of a pack
        /// \param buffer
        template<typename... A>
        static void PackDeleter(const void* buffer) {
            reinterpret_cast<const ParameterStack<A...>*>(buffer)->~ParameterStack();
        }

        /// Pack copier, creates a copy of a pack
        /// \param dest
        /// \param source
        template<typename... A>
        static void PackCopier(void *dest, const void* source) {
            new (dest) ParameterStack<A...>(*reinterpret_cast<const ParameterStack<A...>*>(source));
        }

        template<typename T0, typename T1, typename... TX>
        static void PopulateRTTIArguments(ParameterStackRTTI &rtti, UInt8 index) {
            PopulateRTTIArguments < T0 > (rtti, index);
            PopulateRTTIArguments < T1, TX...>(rtti, index + 1);
        }

        template<typename T0>
        static void PopulateRTTIArguments(ParameterStackRTTI &rtti, UInt8 index) {
            rtti.parameterTypes[index] = GetStaticTypeName<T0>();
        }

        template<typename... A>
        static ParameterStackRTTI ParameterRTTI() {
            static ParameterStackRTTI rtti = [] {
                ParameterStackRTTI value{};
                value.parameterCount = sizeof...(A);
                value.parameterTypes = new const char*[value.parameterCount];
                value.signatureTypeName = GetStaticPackTypeName<A...>();
                if constexpr(sizeof...(A) > 0) {
                    PopulateRTTIArguments<A...>(value, 0);
                }
                return value;
            }();

            return rtti;
        }

        std::size_t size = 0;
        char _buffer[kDefaultStackSize] = {};
        char* buffer = _buffer;
        bool isValid = false;
    };
}