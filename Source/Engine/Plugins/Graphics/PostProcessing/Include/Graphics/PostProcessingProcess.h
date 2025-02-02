//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IPostProcessingProcess.h"

namespace Djinn::Graphics {
    class Pipeline;

    class PostProcessingProcess : public IPostProcessingProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        PostProcessingProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual CameraPacket *GetPacket() override;
        virtual PostProcessingResources GetResources() override;
        virtual COM::Result Build(CameraPacket* packet, const PostProcessingResources& resources, Graphics::PassBuilder builder, double delta) override;
        virtual IPipeline *GetPipeline() override;

    private:
        CameraPacket* packet;
        PostProcessingResources resources;
        Pipeline *pipeline = nullptr;
    };
}