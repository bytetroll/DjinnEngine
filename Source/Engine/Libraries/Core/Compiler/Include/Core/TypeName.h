//
// Created by Dev on 3/14/2018.
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