//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <Game/ISceneActor.h>

namespace Djinn::Game {
    class ILocation : public ISceneActor {
    public:
        DJINN_COM_INTERFACE();

        ILocation(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneActor(registry, outer, host) {

        }

        /// Add an actor to this location
        /// \param actor
        virtual void AddActor(ISceneActor* actor) = 0;

        /// Remove an actor from this location
        /// \param actor
        virtual void RemoveActor(ISceneActor* actor) = 0;
    };
}