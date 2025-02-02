//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "ILightingProcess.h"

namespace Djinn::Graphics {
    class Pipeline;

    class LightingProcess : public ILightingProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        LightingProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Build(CameraPacket* packet, const LightingResources& resources, Graphics::PassBuilder builder, double delta) override;
        virtual CameraPacket *GetPacket() override;
        virtual IPipeline *GetPipeline() override;
        virtual const LightingResources &GetResources() override;

    private:
        LightingResources resources;
        Pipeline *pipeline = nullptr;
        CameraPacket* packet = nullptr;
    };
}