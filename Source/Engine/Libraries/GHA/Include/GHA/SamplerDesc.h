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

#include "SamplerAddressMode.h"
#include "CompareOp.h"
#include "SamplerFilterMode.h"
#include "SamplerMipMapMode.h"
#include <Core/Hash.h>

namespace Djinn::GHA {
    struct SamplerDesc {
        SamplerDesc() = default;

        SamplerDesc(
                SamplerAddressMode addressU, SamplerAddressMode addressV, SamplerAddressMode addressW,
                SamplerFilterMode magFilter, SamplerFilterMode minFilter,
                SamplerMipMapMode mipMapMode,
                bool anisotropy, float maxAnisotropy,
                bool compare, GHA::CompareOp compareOp,
                float maxLOD, float minLOD, float biasLOD
        ) : addressU(addressU), addressV(addressV), addressW(addressW),
            magFilter(magFilter), minFilter(minFilter), mipMapMode(mipMapMode),
            anisotropy(anisotropy), compareOp(compareOp),
            maxLOD(maxLOD), minLOD(minLOD), biasLOD(biasLOD) {

        }

        SamplerDesc &AddressU(SamplerAddressMode value) {
            addressU = value;
            return *this;
        }

        SamplerDesc &AddressV(SamplerAddressMode value) {
            addressV = value;
            return *this;
        }

        SamplerDesc &AddressW(SamplerAddressMode value) {
            addressW = value;
            return *this;
        }

        SamplerDesc &MagFilter(SamplerFilterMode value) {
            magFilter = value;
            return *this;
        }

        SamplerDesc &MinFilter(SamplerFilterMode value) {
            minFilter = value;
            return *this;
        }

        SamplerDesc &MipMapMode(SamplerMipMapMode value) {
            mipMapMode = value;
            return *this;
        }

        SamplerDesc &Anisotropy(bool value) {
            anisotropy = value;
            return *this;
        }

        SamplerDesc &MaxAnisotropy(float value) {
            maxAnisotropy = value;
            return *this;
        }

        SamplerDesc &Compare(bool value) {
            compare = value;
            return *this;
        }

        SamplerDesc &CompareOp(CompareOp value) {
            compareOp = value;
            return *this;
        }

        SamplerDesc &MaxLOD(float value) {
            maxLOD = value;
            return *this;
        }

        SamplerDesc &MinLOD(float value) {
            minLOD = value;
            return *this;
        }

        SamplerDesc &BiasLOD(float value) {
            biasLOD = value;
            return *this;
        }

        USize Hash() {
            return CombineHash(
                    addressU, addressV, addressW,
                    magFilter, minFilter, mipMapMode,
                    anisotropy, maxAnisotropy,
                    compare, compareOp,
                    maxLOD, minLOD, biasLOD
            );
        }

        // Address modes
        SamplerAddressMode addressU = SamplerAddressMode::eRepeat;
        SamplerAddressMode addressV = SamplerAddressMode::eRepeat;
        SamplerAddressMode addressW = SamplerAddressMode::eRepeat;

        // Filters
        SamplerFilterMode magFilter = SamplerFilterMode::eLinear;
        SamplerFilterMode minFilter = SamplerFilterMode::eLinear;

        // Mip map mode
        SamplerMipMapMode mipMapMode = SamplerMipMapMode::eLinear;

        // Is anisotropy enabled?
        bool anisotropy = false;

        // max anisotropy level
        float maxAnisotropy = 16.0f;

        // Is comparison enabled?
        bool compare = false;

        // Comparison operation
        GHA::CompareOp compareOp = GHA::CompareOp::eAlways;

        // Max LOD
        float maxLOD = 16.0f;

        // Min LOD
        float minLOD = 0.0f;

        // Bias LOD
        float biasLOD = 0.0f;
    };

    namespace Samplers {
        static auto noMip = SamplerDesc(
                SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat,
                SamplerFilterMode::eLinear, SamplerFilterMode::eLinear,
                SamplerMipMapMode::eLinear,
                false, 16.0f,
                false, GHA::CompareOp::eAlways,
                0.0f, 0.0f, 0.0f
        );
        static auto anisotropic = SamplerDesc(
                SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat,
                SamplerFilterMode::eLinear, SamplerFilterMode::eLinear,
                SamplerMipMapMode::eLinear,
                true, 16.0f,
                false, GHA::CompareOp::eAlways,
                16.0f, 0.0f, 0.0f
        );
        static auto point = SamplerDesc(
                SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat,
                SamplerFilterMode::eNearest, SamplerFilterMode::eNearest,
                SamplerMipMapMode::eNearest,
                false, 16.0f,
                false, GHA::CompareOp::eAlways,
                16.0f, 0.0f, 0.0f
        );
        static auto pointNoMip = SamplerDesc(
                SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat, SamplerAddressMode::eRepeat,
                SamplerFilterMode::eNearest, SamplerFilterMode::eNearest,
                SamplerMipMapMode::eNearest,
                false, 16.0f,
                false, GHA::CompareOp::eAlways,
                0.0f, 0.0f, 0.0f
        );
    }
}