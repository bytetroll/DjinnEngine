//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Async/Async.h>

namespace Djinn::Game {
    class IScene;
}

namespace Djinn::Graphics {
    struct ScenePacket;

    class IScenePacketBatcher {
    public:
        DJINN_COM_INTERFACEID();

        /// Build onto the supplied scene packet
        /// \param scene
        /// \param batch
        /// \return
        virtual COM::Result Build(Game::IScene *scene, ScenePacket *batch) = 0;

        /// Asynchronous methods
        Asyncf(Build);
    };
}