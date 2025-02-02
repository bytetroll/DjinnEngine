//
// Created by Dev on 8/20/2018.
//
#pragma once

#include "ILocation.h"
#include <Game/ActorCallback.h>

namespace Djinn::Game {
    class TransformComponent;

    class DJINN_EXPORT_PLUGINS_GAME_SCENE Location : public ILocation {
    public:
        DJINN_COM_CLASS();

        Location(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void AddActor(ISceneActor *actor) override;
        void RemoveActor(ISceneActor *actor) override;
        void StaticInitialize() override;

    private:
        /// Update callback
        /// \param delta
        void Update(double delta);

        ActorCallback callback;
        TransformComponent* transform = nullptr;
    };
}