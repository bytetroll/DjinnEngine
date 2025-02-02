//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Graphics/UIGeometryPacket.h>
#include <Math/Vector4.h>

namespace Djinn::Graphics {
    class IOutput;
}

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIOutputDisplay : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIOutputDisplay(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Set the output
        /// \param texture
        void SetOutput(Graphics::IOutput* output) {
            renderer->output = output;
            renderer->MarkAsInvalidated();
        }

        /// Get the output
        /// \return
        Graphics::IOutput* GetOutput() const {
            return renderer->output;
        }

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            /// Output
            Graphics::IOutput* output = nullptr;
        };

    private:
        Renderer *renderer;
    };
}