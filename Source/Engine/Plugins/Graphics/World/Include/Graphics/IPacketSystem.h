//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <Game/IWorldSystem.h>

namespace Djinn::Graphics {
    class PacketBatch;
    class IPacketBatcher;
    struct WorldPacket;

    class IPacketSystem : public Game::IWorldSystem {
    public:
        DJINN_COM_INTERFACE();

        IPacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldSystem(registry, outer, host) {

        }

        /// Register a batcher
        /// \param batcher
        /// \return
        virtual COM::Result Register(IPacketBatcher* batcher) = 0;

        /// Deregister a batcher
        /// \param batcher
        /// \return
        virtual COM::Result Deregister(IPacketBatcher* batcher) = 0;

        /// Pool world packet
        /// \return
        virtual bool PoolPacket(WorldPacket** out) = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case ISystem::kIID:
                    *out = static_cast<IStepSystem*>(this);
                    return COM::kOK;
                case IStepSystem::kIID:
                    *out = static_cast<IStepSystem*>(this);
                    return COM::kOK;
                case Game::IWorldSystem::kIID:
                    *out = static_cast<Game::IWorldSystem*>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IPacketSystem*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}