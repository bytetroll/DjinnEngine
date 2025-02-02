//
// Created by Dev on 1/28/2018.
//
#pragma once

#include "TextureSourceFormat.h"
#include "TextureCompressionMode.h"
#include "TextureCompileMode.h"
#include <Data/IAssetConfiguration.h>

namespace Djinn::Data {
    class TextureConfiguration : public IAssetConfiguration {
    public:
        DJINN_COM_CLASSID();

        /// Overrides
        COM::ClassID GetClassID() const override {
            return kCID;
        }

        /// Override
        USize Hash() override {
            return CombineHash(sourceFormat, generateMipMaps, compressionMode);
        }

        /// Override
        IAssetConfiguration *Copy() override {
            return new TextureConfiguration(*this);
        }

        /// Source format of the texture
        TextureSourceFormat sourceFormat;

        /// Generate mip maps, may be ignored on some texture types
        bool generateMipMaps = true;

        /// Compression mode, may be ignored on some texture types
        TextureCompressionMode compressionMode = TextureCompressionMode::eBC7;

        /// Compilation mode
        TextureCompileMode mode = TextureCompileMode::eInline;
    };
}