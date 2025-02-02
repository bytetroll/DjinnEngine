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
 on 11/23/2017.
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