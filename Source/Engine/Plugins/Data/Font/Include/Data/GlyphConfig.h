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
#pragma once

#include <Math/Vector2.h>
#include <Core/ArrayView.h>

namespace Djinn::Data {
    struct GlyphConfig {
        /// The base offset of this glyph
        Math::Vector2 offset;

        /// The ranges of this glyph
        Math::Vector2 begin;
        Math::Vector2 end;

        /// The uv offsets of this glyph
        Math::Vector2 uvBegin;
        Math::Vector2 uvEnd;

        /// The advance of this glyph
        float advance;

        /// Character of this glyph
        char character;
    };

    struct GlyphRender {
        /// Base glyph
        GlyphConfig config;

        /// Position of this glyph
        Math::Vector2 offset;

        /// Size of this glyph
        Math::Vector2 size;
    };

    struct FontRenderConfig {
        FontRenderConfig &Size(float size) {
            this->size = size;
            return *this;
        }

        FontRenderConfig &LineSeperation(float lineSeperation) {
            this->lineSeperation = lineSeperation;
            return *this;
        }

        /// Size of this font
        float size = 12.0f;

        /// Line seperation
        float lineSeperation = 3;
    };

    struct FontRenderRequest {
        FontRenderRequest() = default;

        FontRenderRequest(const Core::AppendArrayView<GlyphRender>& glyphs) : glyphs(glyphs) {

        }

        /// Rendered glyphs
        Core::AppendArrayView<GlyphRender> glyphs;

        /// Final glyph offset
        Math::Vector2 offset;
    };
}
