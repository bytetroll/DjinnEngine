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
 on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Graphics/UIGeometryPacket.h>
#include <Math/Vector4.h>
#include <UI/UIMLAnnotation.h>
#include <Core/Buffer.h>

namespace Djinn::Data {
    class IFontAsset;
}

namespace Djinn::UI {
    ClassAttr(UIML::Class("label"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UILabel : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UILabel(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        bool Cache() override;
        void StyleEvent(const ElementStyle &style) override;

    public:
        /// Set font asset
        /// \param font
        /// \return
        void SetFont(const Data::Asset<Data::IFontAsset>& font) {
            renderer->font = font;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Set the text value of this label
        /// \param value
        /// \return
        Attr(UIML::Setter("text"))
        void SetText(const Core::StringView &text) {
            renderer->text = text.ToString();
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Set font size
        /// \param size
        void SetFontSize(float size) {
            renderer->fontSize = size;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Set line seperation
        /// \param lineSeperation
        /// \return
        void SetLineSeperation(float lineSeperation) {
            renderer->lineSeperation = lineSeperation;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Set the color
        /// \param color
        void SetColor(const Math::Vector4 &color) {
            renderer->color = color;
            renderer->MarkAsInvalidated();
        }

        /// Set the smoothing
        /// \param smoothing
        void SetSmoothing(float smoothing) {
            renderer->smoothing = smoothing;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Get text
        /// \return
        const Core::String& GetText() {
            return renderer->text;
        }

        /// Get the font
        /// \return
        const Data::Asset<Data::IFontAsset>& GetFont() {
            return renderer->font;
        }

        /// Get smoothing
        /// \return
        float GetSmoothing() {
            return renderer->smoothing;
        }

        /// Get the color
        /// \return
        Math::Vector4 GetColor() {
            return renderer->color;
        }

        /// Get font size
        float GetFontSize() {
            return renderer->fontSize;
        }

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            /// Current font
            Data::Asset<Data::IFontAsset> font = nullptr;

            /// Color
            Math::Vector4 color = Math::Vector4(1, 1, 1, 1);

            /// Smoothing of this font
            float smoothing = 2.0f;

            /// Size of this font
            float fontSize = 12.0f;

            /// Line seperation
            float lineSeperation = 3;

            /// Current text
            Core::String text;

            /// Glyph Cache
            Core::Buffer<Data::GlyphRender> glyphRenders;
        };

    private:
        Renderer *renderer;
    };
}