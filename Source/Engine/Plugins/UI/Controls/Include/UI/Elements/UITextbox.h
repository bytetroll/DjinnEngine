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
#include <Core/Delegate.h>
#include <Math/Vector2I.h>
#include <Game/ActorEvent.h>
#include <UI/UIMLEvent.h>
#include <Data/Asset.h>
#include <Data/GlyphConfig.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Core/Buffer.h>

namespace Djinn::Data {
    class IFontAsset;
    class ITextureAsset;
    class IAssetHost;
}

namespace Djinn::UI {
    /// Delegates
    using UITextboxChangedDelegate = Core::Delegate<void(float value)>;

    ClassAttr(UIML::Class("textbox"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UITextbox : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UITextbox(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Overrides
        void StaticInitialize() override;
        bool Cache() override;

    public:
        /// Set font asset
        /// \param font
        /// \return
        void SetFont(const Data::Asset<Data::IFontAsset> &font) {
            renderer->font = font;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Set font asset by path
        /// \param font
        /// \return
        Attr(UIML::Setter("font"))
        void SetFont(const Core::StringView &path);

        /// Set the button text
        /// \param text
        Attr(UIML::Setter("text"))
        void SetText(const Core::StringView &text);

        /// Get the button text
        /// \return
        Attr(UIML::Getter("text"))
        Core::StringView GetText() const;

        /// Set the hint text
        /// \param text
        Attr(UIML::Setter("hint"))
        void SetHint(const Core::StringView &text);

        /// Get the hint text
        /// \return
        Attr(UIML::Getter("hint"))
        Core::StringView GetHint() const;

        /// Set the texture of this image
        /// \param texture
        void SetColor(const Math::Vector4 &color) {
            renderer->color = color;
            renderer->MarkAsInvalidated();
        }

        /// Get the texture
        /// \return
        const Math::Vector4 &GetColor() const {
            return renderer->color;
        }

        /// Set the texture of this image
        /// \param texture
        void SetHoverTint(const Math::Vector4 &color) {
            renderer->hoverTint = color;
            renderer->MarkAsInvalidated();
        }

        /// Get the texture
        /// \return
        const Math::Vector4 &GetHoverTint() const {
            return renderer->hoverTint;
        }

    public:
        /// Events
        Game::ActorEvent<UITextboxChangedDelegate> pressed;

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            UInt32 GetGlyphIndex(float x) {
                if (glyphRenders.Size() == 0 || x < glyphRenders[0].offset.x + glyphRenders[0].size.x * fontSize * 0.5f) {
                    return 0;
                }
                for (USize i = 0; i < glyphRenders.Size(); i++) {
                    float half = glyphRenders[i].size.x * fontSize * 0.5f;
                    if ((x >= glyphRenders[i].offset.x) && (
                            x <= (glyphRenders[i].offset.x + glyphRenders[i].size.x * fontSize) ||
                            (i != glyphRenders.Size() - 1 && x <= glyphRenders[i + 1].offset.x)
                    )) {
                        if (x < glyphRenders[i].offset.x + half) {
                            return static_cast<UInt32>(i - 1);
                        }
                        return static_cast<UInt32>(i);
                    }
                }
                return glyphRenders.Size();
            }

            /// Current font
            Data::Asset<Data::IFontAsset> font = nullptr;

            /// Window texture
            Data::Asset<Data::ITextureAsset> texture = nullptr;

            /// Color
            Math::Vector4 color = Math::Vector4(0.2f, 0.2f, 0.2f, 1);

            /// Hover tint
            Math::Vector4 hoverTint = Math::Vector4(1.2f, 1.2f, 1.2f, 1.2f);

            /// Smoothing of this font
            float smoothing = 2.0f;

            /// Size of this font
            float fontSize = 12.0f;

            /// Line seperation
            float lineSeperation = 3;

            /// Is this Textbox being hovered?
            bool isHovering{false};

            /// Is this Textbox being pressed?
            bool isPressing{false};

            /// Is this Textbox being edited?
            bool isEditing{false};

            /// Selected indices
            Math::Vector2I selectionIndices = {0, 0};

            /// Current text
            Core::String text;

            /// Hint text
            Core::String hint;

            /// Glyph Cache
            Core::Buffer<Data::GlyphRender> glyphRenders;
            Core::Buffer<Data::GlyphRender> hintGlyphRenders;
        };

    private:
        Renderer *renderer;
        Data::IAssetHost *host;
    };
}