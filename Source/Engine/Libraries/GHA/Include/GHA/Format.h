#pragma once

#include <Core/BitSet.h>

namespace Djinn {
    namespace GHA {
        enum class Format {
            eUndefined,
            eA2R10G10B10Unorm,
            eR8G8B8A8Srgb,
            eR8G8B8A8Snorm,
            eR8G8B8A8Unorm,
            eB8G8R8A8Unorm,
            eR16G16B16A16F,
            eR16G16B16A16I,
            eR16G16B16A16SNorm,
            eR16G16B16A16U,
            eR16G16B16A16UNorm,
            eR32G32B32A32F,
            eR32G32B32A32I,
            eR32G32B32A32U,
            eR32G32B32F,
            eR32G32B32I,
            eR32G32B32U,
            eR32G32F,
            eR32G32I,
            eR32G32U,
            eR32F,
            eR32I,
            eR32U,
            eR8Unorm,
            eR8G8B8Snorm,
            eB8G8R8Snorm,
            eB8G8R8A8Snorm,
            eR8G8B8Unorm,
            eB8G8R8Unorm,
            eR16G16B16F,
            eR16G16B16U,
            eD32F,
            eBC1,
            eBC2,
            eBC3,
            eBC4,
            eBC5,
            eBC6H,
            eBC7,
            eASTC,
            eCount
        };

        static bool IsCompressed(GHA::Format format) {
            switch (format) {
                case Format::eBC1:
                case Format::eBC2:
                case Format::eBC3:
                case Format::eBC4:
                case Format::eBC5:
                case Format::eBC6H:
                case Format::eBC7:
                case Format::eASTC:
                    return true;
                default:
                    return false;
            }
        }

        // Bitset
        DJINN_CORE_BITSET(Format);
    }
}