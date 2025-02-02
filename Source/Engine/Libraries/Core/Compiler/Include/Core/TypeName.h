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
 on 3/14/2018.
//
#pragma once

#include <Core/Size.h>
#include <cstring>

namespace Djinn {
    /// Get type name
    /// \return
    template<class T>
    char* GetTypeName() {
        const char* ptr = __PRETTY_FUNCTION__;
        const char* begin = std::strstr(ptr, "=") + 2;
        USize length = std::strlen(begin) - 1;
        char* type = new char[length + 1];
        type[length] = 0;
        std::memcpy(type, begin, length);
        return type;
    }

    /// Get type name
    /// \return
    template<typename... T>
    char* GetPackTypeName() {
        const char* ptr = __PRETTY_FUNCTION__;
        const char* begin = std::strstr(ptr, "=") + 2;
        USize length = std::strlen(begin) - 1;
        char* type = new char[length + 1];
        type[length] = 0;
        std::memcpy(type, begin, length);
        return type;
    }

    /// Get type name
    /// \return
    template<class T>
    char* GetStaticTypeName() {
        static auto ptr = GetTypeName<T>();
        return ptr;
    }

    /// Get type name
    /// \return
    template<class... T>
    char* GetStaticPackTypeName() {
        static auto ptr = GetPackTypeName<T...>();
        return ptr;
    }

    /// Get type name
    /// \return
    template<typename T, T V>
    char* GetTypeName() {
        const char* ptr = __PRETTY_FUNCTION__;
        const char* begin = std::strstr(ptr, "V =") + 4;
        USize length = std::strlen(begin) - 2;
        char* type = new char[length + 1];
        type[length] = 0;
        std::memcpy(type, begin, length);
        return type;
    }

    template<typename T>
    struct TQualifiedThisRemoveRef
    {
        using Type = T;
    };

    template<typename T>
    struct TQualifiedThisRemoveRef<T&>
    {
        using Type = T;
    };

    template<typename T>
    using QualifiedThisRemoveRef = typename TQualifiedThisRemoveRef<T>::Type;

    /// This typename
#define DJINN_QUALIFIED_THIS Djinn::QualifiedThisRemoveRef<decltype(*this)>
}