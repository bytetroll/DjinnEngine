//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IUIProcess.h"

namespace Djinn::Graphics {
    class Pipeline;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_UI UIProcess : public IUIProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        UIProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual Texture GetDepthTexture() override;
        virtual COM::Result Build(UIPacket *batch, const Texture &depthTexture, Graphics::PassBuilder builder, double delta) override;
        virtual IPipeline *GetPipeline() override;
        virtual UIPacket *GetPacket() override;

    private:
        UIPacket *packet;
        Pipeline *pipeline = nullptr;
        IGraphicsSystem *system;
        Texture depthTexture;
    };
}