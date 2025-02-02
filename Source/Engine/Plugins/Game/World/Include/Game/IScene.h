//
// Created by Dev on 8/25/2018.
//
#pragma once

#include <Async/Async.h>
#include <COM/IUnknown.h>

namespace Djinn::Game {
    class IWorld;
    class ISceneState;

    class IScene : public COM::TUnknown<IScene> {
    public:
        DJINN_COM_INTERFACE();

        IScene(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Build this scene
        /// \return
        virtual COM::Result Build() = 0;

        /// Initialize this scene
        /// \param world - World this scene belongs to
        /// \param state
        /// \return
        virtual COM::Result Initialize(IWorld* world, ISceneState* state) = 0;

        /// Get the world this scene belongs to
        /// \return
        virtual IWorld* GetWorld() = 0;

        /// Get the scene state
        /// \return
        virtual ISceneState* GetState() = 0;

        /// Async helpers
        Asyncf(Build);
    };
}