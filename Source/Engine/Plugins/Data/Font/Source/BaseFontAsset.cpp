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
 on 2/21/2018.
//

#include <Data/BaseFontAsset.h>

DJINN_NS2(Data);

BaseFontAsset::BaseFontAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFontAsset(registry, outer, host) {

}

ITextureAsset *BaseFontAsset::GetTexture() {
    return textureAsset;
}

COM::Result BaseFontAsset::GetGlyphs(const Core::StringView& text, GlyphConfig *out) {
    for (UInt32 i = 0; i < text.Length(); i++) {
        auto it = glyphMap.Find(text[i]);
        if (it == glyphMap.End()) {
            continue;
        }

        out[i] = (*it).second;
    }

    // OK
    return COM::kOK;
}

COM::Result BaseFontAsset::RenderGlyphs(const Core::StringView& text, const FontRenderConfig& config, FontRenderRequest& request) {
    // Must have enough space for render
    DJINN_ASSERT(request.glyphs.GetUnderlyingView().Size() >= text.Length(), "Cannot render font glyphs into an array with less elements than the requested text");

    // Determine render scale
    float renderScale = config.size / GetGlyphSize();

    // Process all glyphs
    request.offset = {0, 0};
    for (USize i = 0; i < text.Length(); i++) {
        auto it = glyphMap.Find(text[i]);
        if (it == glyphMap.End()) {
            continue;
        }

        const GlyphConfig& glyphConfig = (*it).second;

        switch (text[i]) {
            case '\n': {
                request.offset.x = 0;
                request.offset.y += config.size + config.lineSeperation;
                break;
            }
            case '\t': {
                request.offset.x += config.size * 4;
                break;
            }
            default: {
                GlyphRender render;
                render.config = glyphConfig;
                render.offset = request.offset;
                render.size = Math::Vector2(renderScale, renderScale);
                request.glyphs.Add(render);

                request.offset.x += glyphConfig.advance * renderScale;
            }
        }
    }

    // OK
    return COM::kOK;
}

UInt32 BaseFontAsset::GetGlyphSize() {
    return glyphSize;
}
