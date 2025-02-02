//
// Created by Dev on 8/29/2017.
//
#pragma once

#include <cstddef>

namespace NyLang {
    template <class T, class PHANTOM_TAG>
    struct UniqueID {
    public:
        using Type = T;

        UniqueID(std::nullptr_t _ = nullptr) : value(T()), set(0) {

        }

        explicit constexpr UniqueID(T value) : value(value), set(true) {
            // ...
        }

        constexpr UniqueID(const UniqueID& other) : value(other.value), set(other.set) {
            // ...
        }

        UniqueID& operator=(const UniqueID& other) {
            value = other.value;
            set = other.set;
            return *this;
        }

        // Get ID
        constexpr T Get() const {
            return value;
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

        // Comparators
        bool operator ==(const UniqueID& other) const { return value == other.value; }
        bool operator !=(const UniqueID& other) const { return value != other.value; }
        bool operator >=(const UniqueID& other) const { return value >= other.value; }
        bool operator <=(const UniqueID& other) const { return value <= other.value; }
        bool operator >(const UniqueID& other) const { return value > other.value; }
        bool operator <(const UniqueID& other) const { return value < other.value; }

    private:
        // Is set
        bool set;

        // Value
        T value;
    };

    // Declare UniqueID
#define LANG_UNIQUEID(NAME, TYPE) struct NAME##Tag; using NAME = NyLang::UniqueID<TYPE, NAME##Tag>
}