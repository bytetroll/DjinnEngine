#pragma once

#include <Core/Size.h>
#include "CompileHash.h"

namespace Djinn {
    // Default hash, attempts to convert type to USize type
    template<typename T>
    struct Hasher {
        USize operator()(const T& value) const {
            return static_cast<USize>(value);
        }
    };

    // Hash helper
    template <typename T>
    inline static USize Hash(const T& value) {
        return Hasher<T>{}(value);
    }

    // Combine hash helper
    template <typename T0>
    inline static USize CombineHash(T0 a) {
        return Hash(a);
    }

    // Combine hash helper
    template <typename T0, typename T1, typename... TX>
    inline static USize CombineHash(T0 a, T1 b, TX... x) {
        return CombineHash(a) ^ (CombineHash(b, x...) + 0x9e3779b9 + (CombineHash(a)<<6) + (CombineHash(a)>>2));
    }

    // Common integral hashes
    template<> struct Hasher<Int8> { USize operator()(Int8 value) const { return value; }};
    template<> struct Hasher<Int16> { USize operator()(Int16 value) const { return value; }};
    template<> struct Hasher<Int32> { USize operator()(Int32 value) const { return value; }};
    template<> struct Hasher<Int64> { USize operator()(Int64 value) const { return value; }};
    template<> struct Hasher<UInt8> { USize operator()(UInt8 value) const { return value; }};
    template<> struct Hasher<UInt16> { USize operator()(UInt16 value) const { return value; }};
    template<> struct Hasher<UInt32> { USize operator()(UInt32 value) const { return value; }};
    template<> struct Hasher<UInt64> { USize operator()(UInt64 value) const { return value; }};
    template<> struct Hasher<char*> { USize operator()(char* str) const { return Core::ComputeCRC64(str); }};
    template<> struct Hasher<const char*> { USize operator()(const char* str) const { return Core::ComputeCRC64(str); }};

    // Common floating point hashes
    template<> struct Hasher<float> { USize operator()(float value) const { auto alias = static_cast<void*>(&value); return *static_cast<UInt32*>(alias); } };
    template<> struct Hasher<double> { USize operator()(double value) const {  auto alias = static_cast<void*>(&value); return *static_cast<USize*>(alias); } };

    // Pointer hash
    template<typename T> struct Hasher<T*> { USize operator()(T* ptr) const { return reinterpret_cast<USize>(ptr); }};
}