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
 on 6/1/2018.
//
#pragma once

#include <COM/ID.h>

namespace Djinn::Reflection {
    struct ClassID {
    public:
        ClassID(std::nullptr_t * = nullptr) : set(false) {

        }

        ClassID(const COM::ClassID& cid) : ClassID(cid.Get()) {

        }

        ClassID(const COM::InterfaceID & iid) : ClassID(iid.Get()) {

        }

        explicit ClassID(USize uid) : uid(uid), set(true) {

        }

        USize Get() const {
            return uid;
        }

        bool IsSet() const {
            return set;
        }

        COM::ClassID AsCOMClassID() const {
            return COM::ClassID(uid);
        }

        COM::InterfaceID AsCOMInterfaceID() const {
            return COM::InterfaceID(uid);
        }

        // Bool operator
        operator bool() const {
            return set;
        }

        // Comparators
        bool operator==(const ClassID &other) const { return uid == other.uid; }
        bool operator!=(const ClassID &other) const { return uid != other.uid; }
        bool operator>=(const ClassID &other) const { return uid >= other.uid; }
        bool operator<=(const ClassID &other) const { return uid <= other.uid; }
        bool operator>(const ClassID &other) const { return uid > other.uid; }
        bool operator<(const ClassID &other) const { return uid < other.uid; }

    private:
        USize uid = 0;

        // Is set
        bool set;
    };

    /// Get id of typename
    /// \tparam T
    /// \return
    template<typename T>
    static ClassID ToID() {
        return ClassID(GetTypeNameCRC64<T>());
    }

    /// Get id from string
    /// \return
    static ClassID ToID(const char* str) {
        return ClassID(Core::ComputeCRC64(str));
    }

    /// Get id from class id
    /// \return
    static ClassID ToID(COM::ClassID cid) {
        return ClassID(cid.Get());
    }

    /// Get id from interface id
    /// \return
    static ClassID ToID(COM::InterfaceID iid) {
        return ClassID(iid.Get());
    }
}

namespace Djinn {
    /// Common hashes
    template<>
    struct Hasher<Reflection::ClassID> {
        USize operator()(const Reflection::ClassID &value) const { return Hasher<UInt64>{}(value.Get()); }
    };
}