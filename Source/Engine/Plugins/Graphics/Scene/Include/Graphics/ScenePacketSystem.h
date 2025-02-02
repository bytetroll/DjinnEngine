//
// Created by Dev on 3/1/2018.
//
#pragma once

#include "IScenePacketSystem.h"

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Graphics {
    class ScenePacketSystem : public IScenePacketSystem {
    public:
        DJINN_COM_CLASS();

        ScenePacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this batcher
        /// \param world
        /// \return
        COM::Result Initialize(Game::IWorld *world);

        /// Overrides
        COM::Result Register(IScenePacketBatcher *batcher) override;
        COM::Result Deregister(IScenePacketBatcher *batcher) override;
        COM::Result Build(WorldPacket* packet) override;

    private:
        Game::IWorld* world;
        Core::Array<IScenePacketBatcher*> batchers;
    };
}