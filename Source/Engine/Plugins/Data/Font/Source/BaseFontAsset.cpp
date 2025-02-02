//
// Created by Dev on 2/21/2018.
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
