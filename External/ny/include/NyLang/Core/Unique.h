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
 on 8/29/2017.
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