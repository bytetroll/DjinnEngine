//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IWorldProcess.h"

namespace Djinn::Graphics {
    class Pipeline;

    class WorldProcess : public IWorldProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        WorldProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Build(WorldPacket *packet, Graphics::PassBuilder builder, double delta) override;
        virtual IPipeline *GetPipeline() override;
        virtual WorldPacket *GetPacket() override;

    private:
        WorldPacket *packet = nullptr;
        Pipeline *pipeline = nullptr;
    };
}