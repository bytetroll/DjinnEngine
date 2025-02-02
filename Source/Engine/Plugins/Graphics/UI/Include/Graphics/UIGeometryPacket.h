//
// Created by Dev on 2/27/2018.
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