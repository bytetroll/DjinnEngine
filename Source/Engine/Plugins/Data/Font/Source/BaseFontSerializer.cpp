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

#include <Data/BaseFontSerializer.h>
#include <Data/BaseFontAsset.h>
#include <Data/ITextureAsset.h>
#include <Host/IRegistry.h>
#include <Serialization/Common/VectorMath.h>

DJINN_NS2(Data);

BaseFontSerializer::BaseFontSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFontSerializer(registry, outer, host) {

}

COM::Result BaseFontSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == BaseFontAsset::kCID);

    // ...
    auto asset = static_cast<BaseFontAsset *>(object);

    // Write texture
    Check(archive << asset->textureAsset);

    // Write misc
    archive << asset->glyphSize;

    // Write glyph configs
    archive << asset->glyphMap.Size();
    for (const auto& glyph : asset->glyphMap) {
        archive << glyph.first;
        archive << glyph.second.character;
        archive << glyph.second.advance;
        archive << glyph.second.begin;
        archive << glyph.second.end;
        archive << glyph.second.offset;
        archive << glyph.second.uvBegin;
        archive << glyph.second.uvEnd;
    }

    // OK
    return COM::kOK;
}

COM::Result BaseFontSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    BaseFontAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}


COM::Result BaseFontSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<BaseFontAsset*>(object);

    // Read texture
    if (COM::CheckedResult result = (archive >> &asset->textureAsset)) {
        return result;
    }

    // Read misc
    archive >> asset->glyphSize;

    // Read glyph config
    USize glyphs;
    archive >> glyphs;
    for (USize i = 0; i < glyphs; i++) {
        char key;
        archive >> key;

        // Read config
        GlyphConfig config;
        archive >> config.character;
        archive >> config.advance;
        archive >> config.begin;
        archive >> config.end;
        archive >> config.offset;
        archive >> config.uvBegin;
        archive >> config.uvEnd;

        // Insert glyph
        asset->glyphMap[key] = config;
    }

    // OK
    return COM::kOK;
}
