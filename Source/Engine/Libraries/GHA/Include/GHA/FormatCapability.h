//
// Created by Dev on 11/25/2017.
//
#pragma once

#include <Core/BitSet.h>

namespace Djinn::GHA {
    enum class FormatCapability {
        eTexture = 1,
        eTextureBuffer = static_cast<int>(eTexture) << 1,
        eVertexBuffer = static_cast<int>(eTextureBuffer) << 1,
        eBlitSrc = static_cast<int>(eVertexBuffer) << 1,
        eBlitDst = static_cast<int>(eBlitSrc) << 1,
        eRenderTarget = static_cast<int>(eBlitDst) << 1,
        eCount = 6
    }; DJINN_CORE_BITSET(FormatCapability);
}