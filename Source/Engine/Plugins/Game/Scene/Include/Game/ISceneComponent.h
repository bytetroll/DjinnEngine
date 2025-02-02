//
// Created by Dev on 8/19/2018.
//
#pragma once

#include <Game/IActorComponent.h>

namespace Djinn::Game {
    class ISceneActor;

    class DJINN_EXPORT_PLUGINS_GAME_SCENE ISceneComponent : public IActorComponent {
    public:
        DJINN_COM_INTERFACE();

        ISceneComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActorComponent(registry, outer, host) {

        }

        /// Get the owning scene actor
        /// \return
        ISceneActor* GetSceneActor();

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance*>(this);
                    return COM::kOK;
                case IActorComponent::kIID:
                    *out = static_cast<IActorComponent*>(this);
                    return COM::kOK;
                case ISceneComponent::kIID:
                    *out = static_cast<ISceneComponent*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}