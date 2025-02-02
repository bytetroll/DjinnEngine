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
#include <Core/Annotation.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::Data {
    class ITextureAsset;
    class IAssetHost;
}

namespace Djinn::UI {
    ClassAttr(UIML::Class("image"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIImage : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIImage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void StyleEvent(const ElementStyle &style) override;

    public:
        /// Set the image geometry as a box
        void SetBoxGeometry() {
            renderer->geometry = Graphics::UIGeometryBox();
        }

        /// Set the image geometry as a margin box
        /// \param margin
        void SetMarginGeometry(const Math::Vector2 &margin) {
            renderer->geometry = Graphics::UIGeometryMarginBox({}, margin);
        }

        /// Set the texture of this image
        /// \param path
        Attr(UIML::Setter("path"))
        void LoadTexture(const Core::StringView& path);

        /// Set the texture of this image
        /// \param texture
        void SetTexture(const Data::Asset<Data::ITextureAsset> &texture) {
            renderer->texture = texture;
            renderer->MarkAsInvalidated();
        }

        /// Get the texture
        /// \return
        const Data::Asset<Data::ITextureAsset> &GetTexture() const {
            return renderer->texture;
        }

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

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            /// Texture
            Data::Asset<Data::ITextureAsset> texture = nullptr;

            /// Geometry template
            Graphics::UIGeometryPacket geometry = Graphics::UIGeometryBox();

            /// Color
            Math::Vector4 color = {1, 1, 1, 1};
        };

    private:
        Renderer *renderer;
        Data::IAssetHost *host;
    };
}