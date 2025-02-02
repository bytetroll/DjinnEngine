//
// Created by Dev on 1/28/2018.
//
#pragma once

#include "FontSourceFormat.h"
#include <Data/IAssetConfiguration.h>

namespace Djinn::Data {
    class FontConfiguration : public IAssetConfiguration {
    public:
        DJINN_COM_CLASSID();

        /// Override
        COM::ClassID GetClassID() const override {
            return kCID;
        }

        /// Override
        USize Hash() override {
            return CombineHash(sourceFormat, glyphRenderSize, glyphSize);
        }

        /// Override
        IAssetConfiguration *Copy() override {
            return new FontConfiguration(*this);
        }

        /// Source format of the Font
        FontSourceFormat sourceFormat;

        /// Render size of each glyph
        UInt32 glyphRenderSize = 96;

        /// Final size of each glyph
        UInt32 glyphSize = 32;
    };
}