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
 on 10/23/2017.
//
#pragma once

#include <Core/Array.h>
#include <COM/Result.h>
#include <Serialization/Archive.h>

namespace Djinn {
    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const char* str) {
        USize len = std::strlen(str);
        archive << len;
        archive.GetStream()->Write(str, len);
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, char*& str) {
        USize length;
        archive >> length;
        str = new char[length + 1];
        archive.GetStream()->Read(str, length);
        str[length] = 0;
    }

    /// Serialization helper
    static void operator<<(Serialization::Archive &archive, const Core::String& str) {
        archive << str.Length();
        archive.GetStream()->Write(str.AsCharArray(), str.Length());
    }

    /// Serialization helper
    static void operator>>(Serialization::Archive &archive, Core::String& str) {
        unsigned int length;
        archive >> length;
        str.Reserve(length);
        archive.GetStream()->Read(str.Ptr(), length);
    }

    /// Serialization helper
    template<typename T>
    static COM::Result operator<<(Serialization::Archive &archive, const Core::Array<T> &array) {
        archive << COM::SafeClassID<T> << array.Size();

        // Write
        for (int i = 0; i < array.Size(); i++) {
            if (!(archive << array[i])) {
                return COM::kError;
            }
        }
        return COM::kOK;
    }

    /// Serialization helper
    template<typename T>
    static COM::Result operator>>(Serialization::Archive &archive, Core::Array<T> &out) {
        UInt64 id;
        archive >> id;
        USize size;
        archive >> size;

        // ...
        out.Resize(size);

        // Types must match
        if (COM::SafeClassID<T>.Get() != id) {
            return COM::kError;
        }

        // Read
        for (Djinn::USize i = 0; i < size; i++) {
            if (!(archive >> out[i])) {
                return COM::kError;
            }
        }
        return COM::kOK;
    }
}