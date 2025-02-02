//
// Created by Dev on 8/30/2018.
//
#pragma once

#include "ISceneCameraPacketBatcher.h"

namespace Djinn::Game {
    class ICameraHost;

    class SceneCameraPacketBatcher : public ISceneCameraPacketBatcher {
    public:
        DJINN_COM_CLASS();

        SceneCameraPacketBatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this batcher
        /// \param host
        /// \return
        COM::Result Initialize(ICameraHost* host);

        /// Overrides
        COM::Result Build(Game::IScene *scene, Graphics::ScenePacket *batch) override;

    private:
        ICameraHost* host;
    };
}