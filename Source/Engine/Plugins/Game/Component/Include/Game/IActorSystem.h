//
// Created by Dev on 1/17/2018.
//
#pragma once

#include <Game/IWorldSystem.h>

namespace Djinn::Game {
    class IScene;
    class IActor;
    class IActorComponent;
    class IActorSystemInstance;
    class ActorSystemCallback;

    struct DeferredActorEvent {
        Core::WeakPtr<IActorSystemInstance> instance;
        Core::DynamicDelegate delegate;
        Core::DynamicParameterStack stack;
    };

    class IActorSystem : public IWorldSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IActorSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldSystem(registry, outer, host) {
            // ...
        }

        /// Queue an action
        /// \param system
        /// \return
        virtual COM::Result Queue(const DeferredActorEvent &event) = 0;

        /// Create an actor from a class id
        /// \param cid
        /// \param out
        /// \return
        virtual COM::Result CreateActor(IScene* scene, const COM::ClassID &cid, IActor** out) = 0;

        /// Create an actor component from a class id
        /// \param cid
        /// \param out
        /// \return
        virtual COM::Result CreateComponent(IScene* scene, const COM::ClassID &cid, IActor* actor, IActorComponent** out) = 0;

        /// Destroy an actor
        /// \param actor
        /// \return
        virtual COM::Result DestroyActor(IActor* actor) = 0;

        /// Destroy an actor component
        /// \param actor
        /// \return
        virtual COM::Result DestroyComponent(IActorComponent* com) = 0;

        /// Register a callback
        /// \param callback
        /// \return
        virtual COM::Result Register(ActorSystemCallback* callback) = 0;

        /// Create actor helper
        /// \return
        template<typename T>
        COM::Result CreateActor(IScene* scene, T** out) {
            IActor* actor;
            Check(CreateActor(scene, T::kCID, &actor));

            // No cast needed
            *out = static_cast<T*>(actor);
            return COM::kOK;
        }

        /// Create component helper
        /// \return
        template<typename T>
        COM::Result CreateComponent(IScene* scene, T** out) {
            IActor* actor;
            Check(CreateComponent(scene, T::kCID, &actor));

            // No cast needed
            *out = static_cast<T*>(actor);
            return COM::kOK;
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case kIID: {
                    *out = static_cast<IActorSystem*>(this);
                    return COM::kOK;
                }
                case IWorldSystem::kIID: {
                    *out = static_cast<IWorldSystem*>(this);
                    return COM::kOK;
                }
                case IStepSystem::kIID: {
                    *out = static_cast<IStepSystem*>(this);
                    return COM::kOK;
                }
                case Base::ISystem::kIID: {
                    *out = static_cast<Base::ISystem*>(this);
                    return COM::kOK;
                }
                case COM::IUnknown::kIID: {
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                }
                return COM::kUnknownInterface;
            }
            return COM::kUnknownInterface;
        }
    };
}