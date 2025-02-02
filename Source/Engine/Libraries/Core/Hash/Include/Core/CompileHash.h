#pragma once

#include <Core/Size.h>
#include <Core/Types.h>
#include <Core/Common.h>
#include "Core/Detail/CRC16.h"
#include "Core/Detail/CRC32.h"
#include "Core/Detail/CRC64.h"
#include <cstddef>

namespace Djinn {
    namespace Core {
        /// Compute crc from compile time string
        constexpr UInt16 ComputeCRC16(const char *str, UInt16 crc = ~static_cast<UInt16>(0)) { return *str == 0 ? ~crc : ComputeCRC16(str + 1, HashDetail::kCRC16[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
        constexpr UInt32 ComputeCRC32(const char *str, UInt32 crc = ~static_cast<UInt32>(0)) { return *str == 0 ? ~crc : ComputeCRC32(str + 1, HashDetail::kCRC32[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
        constexpr UInt64 ComputeCRC64(const char *str, UInt64 crc = ~static_cast<UInt64>(0)) { return *str == 0 ? ~crc : ComputeCRC64(str + 1, HashDetail::kCRC64[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
        constexpr UInt16 ComputeCRC16(const char *str, const char *end, UInt16 crc = ~static_cast<UInt16>(0)) { return str == end ? ~crc : ComputeCRC16(str + 1, end, HashDetail::kCRC16[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
        constexpr UInt32 ComputeCRC32(const char *str, const char *end, UInt32 crc = ~static_cast<UInt32>(0)) { return str == end ? ~crc : ComputeCRC32(str + 1, end, HashDetail::kCRC32[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
        constexpr UInt64 ComputeCRC64(const char *str, const char *end, UInt64 crc = ~static_cast<UInt64>(0)) { return str == end ? ~crc : ComputeCRC64(str + 1, end, HashDetail::kCRC64[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }

        namespace Detail {
            /// CRC16 compile time hash, does not perform bitwise not
            template<UInt16, char...> struct CRC16;
            template<UInt16 crc, char C> struct CRC16<crc, C> { static constexpr UInt16 kValue = HashDetail::kCRC16[static_cast<unsigned char>(crc) ^ C] ^ (crc >> 8); };
            template<UInt16 crc, char CA, char CB, char... CX> struct CRC16<crc, CA, CB, CX...> { static constexpr UInt16 kValue = CRC16<CRC16<crc, CA>::kValue, CB, CX...>::kValue; };

            /// CRC32 compile time hash, does not perform bitwise not
            template<UInt32, char...> struct CRC32;
            template<UInt32 crc, char C> struct CRC32<crc, C> { static constexpr UInt32 kValue = HashDetail::kCRC32[static_cast<unsigned char>(crc) ^ C] ^ (crc >> 8); };
            template<UInt32 crc, char CA, char CB, char... CX> struct CRC32<crc, CA, CB, CX...> { static constexpr UInt32 kValue = CRC32<CRC32<crc, CA>::kValue, CB, CX...>::kValue; };

            /// CRC64 compile time hash, does not perform bitwise not
            template<UInt64, char...> struct CRC64;
            template<UInt64 crc, char C> struct CRC64<crc, C> { static constexpr UInt64 kValue = HashDetail::kCRC64[static_cast<unsigned char>(crc) ^ C] ^ (crc >> 8); };
            template<UInt64 crc, char CA, char CB, char... CX> struct CRC64<crc, CA, CB, CX...> { static constexpr UInt64 kValue = CRC64<CRC64<crc, CA>::kValue, CB, CX...>::kValue; };

            /// Get typename CRC64 hash without temporary allocations
            /// \tparam T
            /// \return
            template<typename T>
            constexpr UInt64 GetTypeNameCRC64() {
                UInt64 crc = ~static_cast<UInt64>(0);

                const char *ptr = __PRETTY_FUNCTION__;
                while (*ptr != '=') ++ptr;
                ptr += 2;

                for (;; ptr++) {
                    if (*ptr == ']' || *ptr == ';') {
                        return ~crc;
                    } else {
                        crc = Core::HashDetail::kCRC64[static_cast<unsigned char>(crc) ^ *ptr] ^ (crc >> 8);
                    }
                }
            }

            /// Get typename CRC64 hash without temporary allocations
            /// \tparam T
            /// \return
            template<typename T>
            constexpr UInt64 GetSuperTypeNameCRC64() {
                UInt64 crc = ~static_cast<UInt64>(0);

                // GetSuperTypeNameCRC64() [with T = ...::Super::T]
                const char *ptr = __PRETTY_FUNCTION__;
                while (*ptr != '=') ++ptr;
                ptr += 2;

                const char *end = ptr;
                while (*end && *end != ';') ++end;
                while (*end != ':') --end;
                --end;

                return ComputeCRC64(ptr, end);
            }
        }

        /// Template based CRC hashes
        /// \tparam CX
        /*template<char... CX> constexpr UInt16 CRC16 = static_cast<UInt16>(~(Detail::CRC16<~static_cast<UInt16>(0), CX...>::kValue));
        template<char... CX> constexpr UInt32 CRC32 = static_cast<UInt32>(~(Detail::CRC32<~static_cast<UInt32>(0), CX...>::kValue));
        template<char... CX> constexpr UInt64 CRC64 = static_cast<UInt64>(~(Detail::CRC64<~static_cast<UInt64>(0), CX...>::kValue));*/
    }

    /// Intermediate id
    struct AnyHashID {
        constexpr AnyHashID() : id(0), str(nullptr) {

        }

        constexpr AnyHashID(UInt64 id, const char* str = nullptr) : id(id), str(str) {

        }

        constexpr operator UInt64() const {
            return id;
        }

        constexpr UInt64 GetID() const {
            return id;
        }

        constexpr const char* GetString(const char* _default = nullptr) const {
            return str ?: _default;
        }

    private:
        UInt64 id;
        const char* str;
    };

    template<char... C>
    struct ConcatHashStr {
        static constexpr char kStr[sizeof...(C)+1] = { C..., '\0' };
    };

    /// String literals for compile time hashes
    /// Using template rules since constexpr is not guarenteed on literals
    /// \return
    /*template<typename T, T... C> constexpr UInt16 operator "" _crc16() { return Core::CRC16<C...>; }
    template<typename T, T... C> constexpr UInt32 operator "" _crc32() { return Core::CRC32<C...>; }
    template<typename T, T... C> constexpr UInt64 operator "" _crc64() { return Core::CRC64<C...>; }
    template<typename T, T... C> constexpr UInt64 operator "" _hash() { return Core::CRC64<C...>; }
    template<typename T, T... C> constexpr AnyHashID operator "" _id() { return AnyHashID(Core::CRC64<C...>, ConcatHashStr<C...>::kStr); }*/
    constexpr UInt16 operator "" _crc16(const char *str, size_t) { return Core::ComputeCRC16(str); }
    constexpr UInt32 operator "" _crc32(const char *str, size_t) { return Core::ComputeCRC32(str); }
    constexpr UInt64 operator "" _crc64(const char *str, size_t) { return Core::ComputeCRC64(str); }
    constexpr AnyHashID operator "" _id(const char *str, size_t) { return AnyHashID(Core::ComputeCRC64(str), str); }

    /// Get typename CRC64 hash without temporary allocations, guarenteed compile time
    /// \tparam T
    /// \return
    template<typename T>
    constexpr UInt64 GetTypeNameCRC64() {
        constexpr UInt64 hash = Core::Detail::GetTypeNameCRC64<T>();
        return hash;
    }

    /// Get typename CRC64 hash without temporary allocations, guarenteed compile time
    /// \tparam T
    /// \return
    template<typename T>
    constexpr UInt64 GetSuperTypeNameCRC64() {
        constexpr UInt64 hash = Core::Detail::GetSuperTypeNameCRC64<T>();
        return hash;
    }
}