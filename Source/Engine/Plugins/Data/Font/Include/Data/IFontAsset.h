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
 on 1/28/2018.
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