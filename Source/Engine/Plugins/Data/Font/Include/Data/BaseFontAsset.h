//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Core/HashMap.h>
#include <Core/ArrayView.h>
#include "GlyphConfig.h"
#include "IFontAsset.h"

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_FONT BaseFontAsset : public IFontAsset {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        BaseFontAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result GetGlyphs(const Core::StringView& text, GlyphConfig *out) override;
        COM::Result RenderGlyphs(const Core::StringView& text, const FontRenderConfig& config, FontRenderRequest& request) override;
        ITextureAsset *GetTexture() override;
        UInt32 GetGlyphSize() override;

        /// Texture asset
        ITextureAsset *textureAsset = nullptr;

        /// Glyph size
        UInt32 glyphSize = 0;

        /// Glyph map
        Core::HashMap<char, GlyphConfig> glyphMap;
    };
}