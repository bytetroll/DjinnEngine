#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Core/Detail/CRC64.h>
#include <NyLang/Core/Detail/CRC32.h>
#include <NyLang/Core/Detail/CRC16.h>
#include <NyLang/Core/Unique.h>

namespace NyLang {
    // Compute crc from compile time string
    constexpr std::uint16_t ComputeCRC16(const char* str, std::uint16_t crc = ~static_cast<std::uint16_t>(0U)) {  return *str == 0 ? ~crc : ComputeCRC16(str + 1, HashDetail::kCRC16[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
    constexpr std::uint32_t ComputeCRC32(const char* str, std::uint32_t crc = ~static_cast<std::uint32_t>(0U)) {  return *str == 0 ? ~crc : ComputeCRC32(str + 1, HashDetail::kCRC32[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }
    constexpr std::uint64_t ComputeCRC64(const char* str, std::uint64_t crc = ~static_cast<std::uint64_t>(0U)) {  return *str == 0 ? ~crc : ComputeCRC64(str + 1, HashDetail::kCRC64[static_cast<unsigned char>(crc) ^ *str] ^ (crc >> 8)); }

    // Templated version
    template<std::uint16_t, char...>  struct ComputeCTCRC16;
    template<std::uint16_t CRC, char C, char C1, char... CX>  struct ComputeCTCRC16<CRC, C, C1, CX...> { static constexpr std::uint16_t kValue = ComputeCTCRC16<ComputeCTCRC16<CRC, C>::kValue, C1, CX...>::kValue; };
    template<std::uint16_t CRC, char C>  struct ComputeCTCRC16<CRC, C> { static constexpr std::uint16_t kValue = HashDetail::kCRC16[static_cast<unsigned char>(CRC) ^ C] ^ (CRC >> 8); };
    template<std::uint32_t, char...>  struct ComputeCTCRC32;
    template<std::uint32_t CRC, char C, char C1, char... CX>  struct ComputeCTCRC32<CRC, C, C1, CX...> { static constexpr std::uint32_t kValue = ComputeCTCRC32<ComputeCTCRC32<CRC, C>::kValue, C1, CX...>::kValue; };
    template<std::uint32_t CRC, char C>  struct ComputeCTCRC32<CRC, C> { static constexpr std::uint32_t kValue = HashDetail::kCRC32[static_cast<unsigned char>(CRC) ^ C] ^ (CRC >> 8); };
    template<std::uint64_t, char...>  struct ComputeCTCRC64;
    template<std::uint64_t CRC, char C, char C1, char... CX>  struct ComputeCTCRC64<CRC, C, C1, CX...> { static constexpr std::uint64_t kValue = ComputeCTCRC64<ComputeCTCRC64<CRC, C>::kValue, C1, CX...>::kValue; };
    template<std::uint64_t CRC, char C>  struct ComputeCTCRC64<CRC, C> { static constexpr std::uint64_t kValue = HashDetail::kCRC64[static_cast<unsigned char>(CRC) ^ C] ^ (CRC >> 8); };


    // Literal builder
#define LANG_ID(DEPTH, NAME, IDNAME) \
    LANG_UNIQUEID(NAME, std::uint##DEPTH##_t); \
    constexpr NAME operator"" _##IDNAME(const char* str, size_t len) { return NAME(ComputeCRC##DEPTH(str)); }

    // Literals
    constexpr std::uint64_t operator"" _id(const char* str, size_t len) { return ComputeCRC64(str); }
}