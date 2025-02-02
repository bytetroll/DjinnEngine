//
// Created by Dev on 11/26/2017.
//
#pragma once

namespace Djinn::GHA {
    enum class TextureLayout : unsigned char {
        eUndefined,
        eColorRenderTarget,
        eColorRead,
        eDepthStencilRenderTarget,
        eDepthStencilRead,
        eTransferSrc,
        eTransferDst,
        ePresentSrc,
    };
}