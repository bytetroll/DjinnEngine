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