//
// Created by Dev on 7/13/2018.
//
#pragma once

#include <Game/ISceneActor.h>

namespace Djinn::Game {
    class ICamera;
}

namespace Djinn::Editor {
    class IViewportCamera : public Game::ISceneActor {
    public:
        DJINN_COM_INTERFACE();

        IViewportCamera(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneActor(registry, outer, host) {

        }

        /// Get the underlying camera
        /// \return
        virtual Game::ICamera* GetGameCamera() = 0;
    };
}