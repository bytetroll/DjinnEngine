//
// Created by Dev on 1/28/2018.
//
#pragma once

namespace Djinn::Data {
    enum class TextureCompressionMode {
        eNone,
        eBC1,
        eBC2,
        eBC3,
        eBC4,

        /// RG
        eBC5,

        /// RGBA
        eBC7,

        /// RGBA
        eASTC,
    };
}