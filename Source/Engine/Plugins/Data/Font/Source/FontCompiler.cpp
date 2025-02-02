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

#include <Data/FontCompiler.h>
#include <Base/DefaultLogs.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define SDF_IMPLEMENTATION

#include <Data/Detail/sdf.h>
#include <Data/BaseFontAsset.h>
#include <Data/BaseTextureAsset.h>
#include <Core/Math.h>
#include <Core/Timer.h>

DJINN_NS2(Data);

namespace {
    FT_Library ftLibrary;

    /// Read callback
    unsigned long _FT_Read(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count) {
        auto istr = reinterpret_cast<Core::IStream *>(stream->descriptor.pointer);

        // Seek if needed
        if (offset != istr->GetPosition()) {
            istr->SetPosition(offset);
        }

        // Read
        if (count == 0) {
            return 0;
        } else {
            return static_cast<unsigned long>(istr->Read(buffer, count));
        }
    }

    /// Read callback
    void _FT_Close(FT_Stream stream) {
        // Nothing
    }

    /// Error helper
    const char *_FT_ErrorString(FT_Error err) {
#undef __FTERRORS_H__
#define FT_ERRORDEF(e, v, s)  case e: return s;
#define FT_ERROR_START_LIST     switch (err) {
#define FT_ERROR_END_LIST       }

#include FT_ERRORS_H
        return "(Unknown error)";
    }
}

FontCompiler::FontCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFontCompiler(registry, outer, host) {

}

FontCompiler::~FontCompiler() {
    FT_Done_FreeType(ftLibrary);
}

COM::Result FontCompiler::Initialize() {
    // Initialize freetype
    if (FT_Init_FreeType(&ftLibrary)) {
        ErrorLog(this).Write("Freetype initialization failed");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result FontCompiler::Compile(Core::IStream *stream, IAssetConfiguration *_config, IAsset **out) {
    auto config = reinterpret_cast<FontConfiguration *>(_config);

    // Diagnostic
    Core::Timer timer;
    InfoLog(this).Write("Started font compilation job [", config->Hash(), "]");

    // Create font asset
    BaseFontAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        ErrorLog(this).Write("Failed to create font asset");
        return COM::kError;
    }

    // Create texture asset
    BaseTextureAsset *textureAsset;
    if (!registry->CreateClass(nullptr, &textureAsset)) {
        ErrorLog(this).Write("Failed to create the fonts texture asset");
        return COM::kError;
    }

    // ...
    asset->textureAsset = textureAsset;
	asset->glyphSize = config->glyphSize;

    // FT Face
    FT_Face ftFace;

    // Prepare stream
    FT_StreamRec ftStreamRec = {};
    ftStreamRec.descriptor.pointer = stream;
    ftStreamRec.read = _FT_Read;
    ftStreamRec.close = _FT_Close;
    ftStreamRec.size = 0x7FFFFFFF;
    ftStreamRec.pos = 0;

    // Load font from stream
    FT_Open_Args ftArgs = {};
    ftArgs.flags = FT_OPEN_STREAM;
    ftArgs.stream = &ftStreamRec;
    if (FT_Error error = FT_Open_Face(ftLibrary, &ftArgs, 0, &ftFace)) {
        ErrorLog(this).Write("Could not open freetype face: ", _FT_ErrorString(error));
        return COM::kError;
    }

    // Set char size from config
    if (FT_Error error = FT_Set_Char_Size(ftFace, 0, static_cast<FT_F26Dot6>(config->glyphSize) << 6, config->glyphRenderSize, config->glyphRenderSize)) {
        ErrorLog(this).Write("Could not set freetype char size: ", _FT_ErrorString(error));
        return COM::kError;
    }

    // Set charmap
    if (FT_Error error = FT_Select_Charmap(ftFace, FT_ENCODING_ADOBE_STANDARD)) {
        ErrorLog(this).Write("Could not select a freetype charmap: ", _FT_ErrorString(error));
        return COM::kError;
    }

    // Max dimensions
    UInt32 maxDim = static_cast<UInt32>((1 + (ftFace->size->metrics.height >> 6)) * Math::Ceil(Math::Sqrt(ftFace->num_glyphs)));

    // TODO: Linear solution
    textureAsset->size = GHA::Size(1, 1, 1);
    while (textureAsset->size.width < maxDim) {
        textureAsset->size.width <<= 1;
        textureAsset->size.height <<= 1;
    }

    // Default format
    textureAsset->format = GHA::Format::eR8Unorm;

    // Prepare base mip
    textureAsset->mipMapBlobs.Resize(1);
    textureAsset->mipMapBlobs[0].data.Resize(textureAsset->size.width * textureAsset->size.height);

    // GIndex
    FT_UInt agIndex;
	
	// Temporary map
	Core::HashMap<UInt32, FT_ULong> glyphLUT;
	
	// Create glyph lookup map
    FT_ULong charCode = FT_Get_First_Char(ftFace, &agIndex);
	while (agIndex != 0) {
		if (charCode < 256) {
			glyphLUT[agIndex] = charCode;
		}
		charCode = FT_Get_Next_Char(ftFace, charCode, &agIndex);
	}

    // Compile each glyph
    Math::Vector2 pen;
    for (UInt32 i = 0; i < static_cast<UInt32>(ftFace->num_glyphs); i++) {
        // Load char
        if (FT_Error error = FT_Load_Char(ftFace, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT)) {
            ErrorLog(this).Write("Could not load a freetype char: ", _FT_ErrorString(error));
            return COM::kError;
        }

        // Get bitmap
        FT_Bitmap *ftBitmap = &ftFace->glyph->bitmap;

        // Are we going to overflow?
        if (pen.x + ftBitmap->width >= textureAsset->size.width) {
            pen.x = 0;
            pen.y += ((ftFace->size->metrics.height >> 6) + 1);
        }

        // Copy to texture
        for (UInt32 x = 0; x < ftBitmap->rows; ++x) {
            for (UInt32 y = 0; y < ftBitmap->width; ++y) {
                textureAsset->mipMapBlobs[0].data[(pen.y + x) * textureAsset->size.width + (pen.x + y)] = ftBitmap->buffer[x * ftBitmap->pitch + y];
            }
        }

        // Prepare config
        GlyphConfig glyphConfig;
        glyphConfig.offset = Math::Vector2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top);
        glyphConfig.advance = (ftFace->glyph->advance.x >> 6);
        glyphConfig.begin = pen;
        glyphConfig.uvBegin = pen / Math::Vector2(textureAsset->size.width, textureAsset->size.height);
        glyphConfig.end = (pen + Math::Vector2(ftBitmap->width, ftBitmap->rows));
        glyphConfig.uvEnd = (pen + Math::Vector2(ftBitmap->width, ftBitmap->rows)) / Math::Vector2(textureAsset->size.width, textureAsset->size.height);
		glyphConfig.character = static_cast<char>(i);

        // Insert
        //asset->glyphMap[static_cast<char>(glyphLUT[i])] = glyphConfig;
		if (i < 256) {
			asset->glyphMap[glyphConfig.character] = glyphConfig;
		}

        // Offset
        pen.x += ftBitmap->width + 1;
    }

    // Free face
    if (FT_Error error = FT_Done_Face(ftFace)) {
        ErrorLog(this).Write("Could not close face: ", _FT_ErrorString(error));
        return COM::kError;
    }

    // Perform SDF calculation
    if (!sdfBuildDistanceField(
            reinterpret_cast<unsigned char*>(textureAsset->mipMapBlobs[0].data.Ptr()),
            textureAsset->size.width,
            2.0f,
            reinterpret_cast<unsigned char*>(textureAsset->mipMapBlobs[0].data.Ptr()),
            textureAsset->size.width, textureAsset->size.height,
            textureAsset->size.width
    )) {
        ErrorLog(this).Write("Failed to build signed distance field");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Font compilation finished (", timer.Delta(), "s)");

    // OK
    *out = asset;
    return COM::kOK;
}

