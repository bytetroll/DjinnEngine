//
// Created by Dev on 10/23/2017.
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