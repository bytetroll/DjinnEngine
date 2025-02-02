//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IWorldPass.h"

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Graphics {
    class IWorldProcess;
    class IPacketSystem;
    class IGraphicsSystem;

    class WorldPass : public IWorldPass {
    public:
        DJINN_COM_CLASS();

        WorldPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this pass
        /// \return
        COM::Result Initialize(Game::IWorld* world, IPacketSystem* packetSystem, IWorldProcess* worldProcess);

        /// Overrides
                virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        Game::IWorld* world;
        IGraphicsSystem* graphicsSystem;
        IPacketSystem* packetSystem;
        IWorldProcess* worldProcess;
    };
}