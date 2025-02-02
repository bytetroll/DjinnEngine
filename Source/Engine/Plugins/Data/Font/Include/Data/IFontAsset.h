//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Data/IAsset.h>
#include <Core/StringView.h>
#include "GlyphConfig.h"

namespace Djinn::Data {
    class ITextureAsset;

    class DJINN_EXPORT_PLUGINS_DATA_FONT IFontAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        IFontAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

        }

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAsset::kIID:
                    *out = static_cast<IAsset*>(this);
                    return COM::kOK;
                case IFontAsset::kIID:
                    *out = static_cast<IFontAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

        /// Get glyph data
        /// \param text - All the characters to be pooled
        /// \param count - Number of glyphs
        /// \param out - Extracted glyps, has the same order as the input glyphs
        /// \return
        virtual COM::Result GetGlyphs(const Core::StringView& text, GlyphConfig* out) = 0;

        /// Render glyph data
        /// \param text
        /// \param out
        /// \return
        virtual COM::Result RenderGlyphs(const Core::StringView& text, const FontRenderConfig& config, FontRenderRequest& request) = 0;

        /// Get the texture asset of this font
        /// \return
        virtual ITextureAsset *GetTexture() = 0;

        /// Get glyph size
        /// \return
        virtual UInt32 GetGlyphSize() = 0;
    };
}