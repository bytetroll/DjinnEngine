//
// Created by Dev on 8/18/2018.
//

#include <Game/ActorSystem.h>
#include <Host/IRegistry.h>
#include <Game/IActor.h>
#include <Game/IActorComponent.h>
#include <Game/ActorCallback.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Game);

ActorSystem::ActorSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActorSystem(registry, outer, host) {

}

COM::Result ActorSystem::Queue(const DeferredActorEvent &event) {
    deferredEvents.Add(event);
    return COM::kOK;
}

COM::Result ActorSystem::CreateActor(IScene* scene, const COM::ClassID &cid, IActor **out) {
    IActor *actor;
    CheckMsg(registry->CreateClass(cid, nullptr, &actor), Error, "Failed to create component");

    // Initialize instance
    actor->InitializeSystemInstance(this, scene);
    {
        actor->SetName(actor->GetClassName());
    }

    // Events
    actor->StaticInitialize();
    //actor->Created();

    // Push
    actorCreations.Add(actor);

    // OK
    *out = actor;
    return COM::kOK;
}

COM::Result ActorSystem::CreateComponent(IScene* scene, const COM::ClassID &cid, IActor *actor, IActorComponent **out) {
    IActorComponent *com;
    CheckMsg(registry->CreateClass(cid, nullptr, &com), Error, "Failed to create component");

    // Initialize instance
    com->InitializeSystemInstance(this, scene);
    {
    }

    // Register
    actor->Register(com);

    // Events
    com->StaticInitialize();
    //com->Created();

    // Push
    componentCreations.Add(com);

    // OK
    *out = com;
    return COM::kOK;
}

COM::Result ActorSystem::DestroyActor(IActor *actor) {
    if (actor->IsMarkedForDeletion()) {
        return COM::kOK;
    }
    actorDeletions.Add(actor);
    return COM::kOK;
}

COM::Result ActorSystem::DestroyComponent(IActorComponent *com) {
    if (com->IsMarkedForDeletion()) {
        return COM::kOK;
    }
    componentDeletions.Add(com);
    return COM::kOK;
}

COM::Result ActorSystem::Register(ActorSystemCallback *callback) {
    stagedCallbacks.Add(callback);
    return COM::kOK;
}

COM::Result ActorSystem::Run(StepMode mode, double delta) {
    // Fire events
    // Note: Events may spawn other events, hence the safe iteration
    while (deferredEvents.Size() != 0) {
        auto event = deferredEvents.PopEnd();
        if (event.instance) {
            event.delegate.InvokeStack(event.stack);
        }
    }

    // Handle creations
    while (actorCreations.Size() != 0) {
        auto actor = actorCreations.PopEnd();
        actor->Created();
    }
    while (componentCreations.Size() != 0) {
        auto com = componentCreations.PopEnd();
        com->Created();
    }

    // Handle callback deletions
    for (auto index : deletedCallbacks) {
        callbacks.Remove(index);
    }
    deletedCallbacks.Clear();

    // Merge callbacks
    for (auto &&staged : stagedCallbacks) {
        auto index = callbacks.Add(*staged);
        staged->deleter = [=] { deletedCallbacks.Add(index); };
    }
    stagedCallbacks.Clear();

    // Handle deletions
    while (componentDeletions.Size() != 0) {
        auto com = componentDeletions.PopEnd();
        Sink(com->Release());
    }

    // Handle deletions
    while (actorDeletions.Size() != 0) {
        auto actor = actorDeletions.PopEnd();
        Sink(actor->Release());
    }

    // Fire callbacks
    for (auto &&callback : callbacks) {
        callback.callback.Invoke(delta);
    }

    // OK
    return COM::kOK;
}

COM::Result ActorSystem::Configure(Base::PipelineBuilder &builder) {
    return COM::kOK;
}

StepModeSet ActorSystem::GetStepMode() {
    return StepMode::eIntegral;
}
