//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "ICameraProcess.h"

namespace Djinn::Graphics {
    class Pipeline;

    class CameraProcess : public ICameraProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        CameraProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Build(CameraPacket *packet, Graphics::PassBuilder builder, double delta) override;
        virtual IPipeline *GetPipeline() override;
        virtual CameraPacket *GetPacket() override;

    private:
        CameraPacket *packet = nullptr;
        Pipeline *pipeline = nullptr;
    };
}