//
// Created by Dev on 8/19/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Game {
    class IActorSystem;
    class IScene;
    class IWorld;

    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT IActorSystemInstance : public COM::TUnknown<IActorSystemInstance> {
    public:
        DJINN_COM_INTERFACE();

        IActorSystemInstance(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this instance
        /// \param system
        void InitializeSystemInstance(IActorSystem* system, IScene* scene);

        /// Get the system that owns this actor
        /// \return
        IActorSystem* GetSystem() const {
            return system;
        }

        /// Get the scene that owns this actor
        /// \return
        IScene* GetScene() const {
            return scene;
        }

        /// Get the world that owns this actor
        /// \return
        IWorld* GetWorld() const {
            return world;
        }

        /// Is this instance marked for deletion?
        /// \return
        bool IsMarkedForDeletion() const {
            return isDeleted;
        }

    protected:
        /// System that owns this instance
        IActorSystem* system = nullptr;

        /// Scene this actor belongs to
        IScene* scene = nullptr;

        /// World this actor belongs to
        IWorld* world = nullptr;

        /// Is this instance marked for deletion?
        bool isDeleted = false;
    };
}