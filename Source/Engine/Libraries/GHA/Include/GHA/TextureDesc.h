//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <Core/TypeAssignable.h>
#include "IResource.h"
#include "Format.h"
#include "Size.h"
#include "TextureUsage.h"
#include "TextureFlag.h"
#include "SampleMode.h"
#include "TextureType.h"
#include "TextureLayout.h"

namespace Djinn::GHA {
    struct TextureDesc {
        TextureDesc &Format(GHA::Format value) {
            format = value;
            return *this;
        }

        TextureDesc &Size(GHA::Size value) {
            size = value;
            return *this;
        }

        TextureDesc &Flags(TextureFlagSet value) {
            flags = value;
            return *this;
        }

        TextureDesc &Usage(TextureUsageSet value) {
            usage = value;
            return *this;
        }

        TextureDesc &Type(TextureType value) {
            type = value;
            return *this;
        }

        TextureDesc &Samples(SampleMode value) {
            samples = value;
            return *this;
        }

        TextureDesc &MipLevels(UInt32 value) {
            mipLevels = value;
            return *this;
        }

        TextureDesc &ArrayCount(UInt32 value) {
            arrayCount = value;
            return *this;
        }

        USize Hash() {
            return CombineHash(format, size.Hash(), flags.Get(), usage.Get(), type, samples, mipLevels, arrayCount.IsValid(), arrayCount.Get());
        }

        // Format of this texture
        GHA::Format format = GHA::Format::eR8G8B8A8Srgb;

        // Size of this texture
        GHA::Size size = GHA::Size(128, 128, 1);

        // Flags to this texture
        TextureFlagSet flags;

        // Usage of this texture
        TextureUsageSet usage = TextureUsage::eColor;

        // Type of this texture
        TextureType type = TextureType::e2D;

        // Sample mode of this texture
        SampleMode samples = SampleMode::e1;

        // Mip levels of this texture
        // 0 indicating automatic mip levels
        UInt32 mipLevels = 1;

        // Array dimension
        Core::Optional<UInt32> arrayCount;
    };
}