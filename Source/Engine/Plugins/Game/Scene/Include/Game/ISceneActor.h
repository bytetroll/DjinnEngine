//
// Created by Dev on 8/19/2018.
//
#pragma once

#include <Game/IActor.h>

namespace Djinn::Game {
    class ITransformComponent;

    class DJINN_EXPORT_PLUGINS_GAME_SCENE ISceneActor : public IActor {
    public:
        DJINN_COM_INTERFACE();

        ISceneActor(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActor(registry, outer, host) {

        }

        /// Overrides
        void StaticInitialize() override;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance*>(this);
                    return COM::kOK;
                case IActor::kIID:
                    *out = static_cast<IActor*>(this);
                    return COM::kOK;
                case ISceneActor::kIID:
                    *out = static_cast<ISceneActor*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Set the transform
        /// \param transform
        void SetTransform(ITransformComponent* transform);

        /// Get the transform component
        /// \return
        ITransformComponent *GetTransform() const {
            return transform;
        }

    private:
        /// Root transform component
        Core::WeakPtr<ITransformComponent> transform;
    };
}