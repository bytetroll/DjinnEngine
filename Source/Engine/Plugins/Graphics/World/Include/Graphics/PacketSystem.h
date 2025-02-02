//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <Core/Queue.h>
#include <Core/Array.h>
#include "IPacketSystem.h"
#include "WorldPacket.h"

namespace Djinn::Graphics {
    class IGraphicsSystem;

    class PacketSystem : public IPacketSystem {
    public:
        DJINN_COM_CLASS();

        PacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Run(Game::StepMode mode, double delta) override;
        virtual Game::StepModeSet GetStepMode() override;
        virtual COM::Result Register(IPacketBatcher *batcher) override;
        virtual COM::Result Deregister(IPacketBatcher *batcher) override;
        virtual bool PoolPacket(WorldPacket **out) override;
        virtual COM::Result Configure(Base::PipelineBuilder& builder) override;

    private:
        Platform::SpinMutex packetMtx;
        IGraphicsSystem *graphicsSystem;
        Core::ObjectPool<WorldPacket> packets;
        Core::Queue<WorldPacket *> packetQueue;
        Core::Array<IPacketBatcher *> batchers;
    };
}