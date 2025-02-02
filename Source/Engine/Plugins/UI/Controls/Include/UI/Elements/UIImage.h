//
// Created by Dev on 8/23/2018.
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