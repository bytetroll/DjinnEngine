//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Graphics/UIGeometryPacket.h>
#include <Math/Vector4.h>
#include <Math/Spline.h>

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UISpline : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UISpline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        bool Cache() override;

    public:
        /// Set number of subdivisions
        /// \return
        void SetSubdivisions(UInt32 subdivisions) {
            renderer->subdivisions = subdivisions;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Map control points
        /// \return
        Core::Buffer<Math::Vector2>* MapControlPoints() {
            return &renderer->spline.controlPoints;
        }

        /// Unmap control points
        void UnmapControlPoints() {
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Set the color
        /// \param color
        void SetColor(const Math::Vector4 &color) {
            renderer->color = color;
            renderer->MarkAsInvalidated();
        }

        /// Set the width
        /// \param width
        void SetWidth(float width) {
            renderer->width = width;
            renderer->MarkAsInvalidated();
            MarkAsUncached();
        }

        /// Get width
        /// \return
        float GetWidth() {
            return renderer->width;
        }

        /// Get the color
        /// \return
        Math::Vector4 GetColor() {
            return renderer->color;
        }

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            /// Color
            Math::Vector4 color = Math::Vector4(1, 1, 1, 1);

            /// Width
            float width = 2.0f;

            /// Subdivisions
            UInt32 subdivisions = 4;

            /// Cache
            Math::Spline2 spline;
            Core::Buffer<Graphics::UIGeometryVertex> vertices;
        };

    private:
        Renderer *renderer;
    };
}