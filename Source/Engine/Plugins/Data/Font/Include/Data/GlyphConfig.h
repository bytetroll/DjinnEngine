//
// Created by Dev on 2/21/2018.
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
