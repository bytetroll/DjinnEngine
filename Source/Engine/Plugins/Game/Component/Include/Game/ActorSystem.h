//
// Created by Dev on 8/18/2018.
//
#pragma once

#include <Core/Buffer.h>
#include <Core/IndexArray.h>
#include "IActorSystem.h"
#include "ActorSystemCallback.h"

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT ActorSystem : public IActorSystem {
    public:
        DJINN_COM_CLASS();

        ActorSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        COM::Result Queue(const DeferredActorEvent &event) override;
        COM::Result CreateActor(IScene* scene, const COM::ClassID &cid, IActor **out) override;
        COM::Result CreateComponent(IScene* scene, const COM::ClassID &cid, IActor* actor, IActorComponent **out) override;
        COM::Result DestroyActor(IActor *actor) override;
        COM::Result DestroyComponent(IActorComponent *com) override;
        COM::Result Register(ActorSystemCallback *callback) override;
        COM::Result Run(StepMode mode, double delta) override;
        COM::Result Configure(Base::PipelineBuilder &builder) override;
        StepModeSet GetStepMode() override;

    private:
        Core::IndexArray<ActorSystemCallback> callbacks;
        Core::Array<ActorSystemCallback*> stagedCallbacks;
        Core::Array<USize> deletedCallbacks;
        Core::Buffer<IActor*> actorDeletions;
        Core::Buffer<IActor*> actorCreations;
        Core::Buffer<IActorComponent*> componentCreations;
        Core::Buffer<IActorComponent*> componentDeletions;
        Core::Buffer<DeferredActorEvent> deferredEvents;
    };
}