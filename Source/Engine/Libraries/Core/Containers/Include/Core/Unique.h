#pragma once

#include <cstddef>
#include <stddef.h>
#include <Core/CompileHash.h>
#include <Core/Hash.h>

namespace Djinn {
    namespace Core {
        template<class T, class PHANTOM_TAG>
        class Unique {
        public:
            constexpr Unique(const T& value) : value(value) {
                // ...
            }

            constexpr Unique(const Unique& other) : value(other.value) {
                // ...
            }

            // Value
            T value;
        };

        // Declare Unique
#define DJINN_UNIQUE(NAME, TYPE) struct NAME##Tag; using NAME = Djinn::Core::Unique<TYPE, NAME##Tag>

        template <class T, class PHANTOM_TAG>
        struct UniqueID {
        public:
            using Type = T;

            UniqueID(std::nullptr_t = nullptr) : value(T()), set(0), str(nullptr) {
            }

            explicit constexpr UniqueID(T value, const char* str = nullptr) : value(value), set(true), str(str) {
                // ...
            }

            constexpr UniqueID(const AnyHashID& anyID) : value(anyID.GetID()), set(true), str(anyID.GetString()) {
                // ...
            }

            constexpr UniqueID(const UniqueID& other) : value(other.value), set(other.set), str(other.str) {
                // ...
            }

            UniqueID& operator=(const UniqueID& other) {
                value = other.value;
                set = other.set;
                str = other.str;
                return *this;
            }

            // Get ID
            constexpr T Get() const {
                return value;
            }

            constexpr const char* GetString(const char* _default = nullptr) const {
                return str ?: _default;
            }

            // Implicit ID
            constexpr operator T() const {
                return value;
            }

            // Runtime conversion
            operator UniqueID() const {
                return UniqueID(value);
            };

            // Bool operator
            constexpr explicit operator bool() const {
                return set;
            }

            /// Is this id set?
            /// \return
            constexpr bool IsSet() const {
                return set;
            }

            // Comparators
            bool operator ==(const UniqueID& other) const { return value == other.value; }
            bool operator !=(const UniqueID& other) const { return value != other.value; }
            bool operator >=(const UniqueID& other) const { return value >= other.value; }
            bool operator <=(const UniqueID& other) const { return value <= other.value; }
            bool operator >(const UniqueID& other) const { return value > other.value; }
            bool operator <(const UniqueID& other) const { return value < other.value; }

        private:
            // Value
            T value;

            // Is set
            bool set;

            // String
            const char* str;
        };

        // Declare UniqueID
#define DJINN_UNIQUEID(NAME, TYPE) \
        struct NAME##Tag; using NAME = Djinn::Core::UniqueID<TYPE, NAME##Tag>;

        // Common ids
        DJINN_UNIQUEID(StringID, Int64);
    }

    /// Common hashes
    template<> struct Hasher<Core::StringID> { USize operator()(const Core::StringID& value) { return Hasher<Int64>{}(value.Get()); }};
}