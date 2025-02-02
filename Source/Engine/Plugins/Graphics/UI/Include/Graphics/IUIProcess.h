//
// Created by Dev on 2/24/2018.
//
#pragma once

#include <Graphics/IProcess.h>
#include <Graphics/PassBuilder.h>

namespace Djinn::Graphics {
    struct UIPacket;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_UI IUIProcess : public IProcess {
    public:
        DJINN_COM_INTERFACE();

        IUIProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IProcess(registry, outer, host) {

        }

        /// Build this process
        /// \param builder
        /// \param delta
        /// \return
        virtual COM::Result Build(UIPacket* packet, const Texture& depthTexture, Graphics::PassBuilder builder, double delta) = 0;

        /// Get packet
        /// \return
        virtual UIPacket* GetPacket() = 0;

        /// Get depth texture of all ui elements
        /// \return
        virtual Texture GetDepthTexture() = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                case IProcess::kIID:
                    *out = static_cast<IProcess*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}