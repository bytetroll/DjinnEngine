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
 on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Math/Vector2.h>
#include <Data/GlyphConfig.h>
#include <Data/Asset.h>
#include <Core/ArrayView.h>

namespace Djinn::Data {
    class IFontAsset;
}

namespace Djinn::Graphics {
    struct UIGeometryVertex {
        Math::Vector2 position;
        Math::Vector2 texcoord;
    };

    using UIGeometryIndex = UInt32;

    enum class UIGeometryType : UInt8 {
        eNone,
        eBox,
        eLine,
        eMarginBox,
        eCircle,
        eFont,
        eCustom
    };

    struct UIGeometryBox {
        UIGeometryBox() = default;

        UIGeometryBox(const Math::Vector2& size) : size(size) {

        }

        UIGeometryType type = UIGeometryType::eBox;
        Math::Vector2 size;
    };

    struct UIGeometryLine {
        UIGeometryLine() = default;

        UIGeometryLine(const Core::ArrayView<UIGeometryVertex>& vertices, bool connected, float width) : vertices(vertices), connected(connected), width(width) {

        }

        UIGeometryType type = UIGeometryType::eLine;
        Core::ArrayView<UIGeometryVertex> vertices;
        bool connected = true;
        float width = 2.0f;
    };

    struct UIGeometryMarginBox {
        UIGeometryMarginBox() = default;

        UIGeometryMarginBox(const Math::Vector2& size, const Math::Vector2& margin) : size(size), margin(margin) {

        }

        UIGeometryType type = UIGeometryType::eMarginBox;
        Math::Vector2 size;
        Math::Vector2 margin;
    };

    struct UIGeometryCircle {
        UIGeometryCircle() = default;

        UIGeometryCircle(const Math::Vector2& radii, UInt32 segments) : radii(radii), segments(segments) {

        }

        UIGeometryType type = UIGeometryType::eCircle;
        Math::Vector2 radii;
        UInt32 segments = 32;
    };

    struct UIGeometryCustom {
        UIGeometryType type = UIGeometryType::eCustom;
        Core::ArrayView<UIGeometryVertex> vertices;
        Core::ArrayView<UIGeometryIndex> indices;
    };

    struct UIGeometryFont {
        UIGeometryFont() = default;

        UIGeometryFont(const Core::ArrayView<Data::GlyphRender>& glyphs, const Data::AssetView<Data::IFontAsset>& font) : glyphs(glyphs), font(font) {

        }

        UIGeometryType type = UIGeometryType::eFont;

        /// Glyphs
        Core::ArrayView<Data::GlyphRender> glyphs;

        /// Font asset
        Data::AssetView<Data::IFontAsset> font;
    };

    union UIGeometryPacket {
        UIGeometryPacket() : type(UIGeometryType::eNone) {
            // ...
        }

        UIGeometryPacket(const UIGeometryBox &box) : box(box) {
            // ...
        }

        UIGeometryPacket(const UIGeometryLine &line) : line(line) {
            // ...
        }

        UIGeometryPacket(const UIGeometryMarginBox &marginBox) : marginBox(marginBox) {
            // ...
        }

        UIGeometryPacket(const UIGeometryCircle &circle) : circle(circle) {
            // ...
        }

        UIGeometryPacket(const UIGeometryCustom &custom) : custom(custom) {
            // ...
        }

        UIGeometryPacket(const UIGeometryFont &font) : font(font) {
            // ...
        }

        UIGeometryType type;
        UIGeometryBox box;
        UIGeometryLine line;
        UIGeometryMarginBox marginBox;
        UIGeometryCircle circle;
        UIGeometryCustom custom;
        UIGeometryFont font;
    };
}