//
// Created by Dev on 8/18/2018.
//

#include <Game/ActorCallback.h>
#include <Game/IActorSystem.h>
#include <Game/IActorSystemInstance.h>

DJINN_NS2(Game);

ActorCallback::~ActorCallback() {
    if (this->instance) {
        Deregister();
    }
}

void ActorCallback::SetStage(const ActorCallbackStage &stage) {
    systemCallback.stage = stage;
}

void ActorCallback::SetCallback(const Core::Delegate<void(double)> &callback) {
    systemCallback.callback = callback;
}

void ActorCallback::Register(IActorSystemInstance *instance) {
    if (this->instance == instance) {
        return;
    }

    if (this->instance) {
        Deregister();
    }

    this->instance = instance;
    Sink(instance->GetSystem()->Register(&systemCallback));
}

void ActorCallback::Deregister() {
    if (systemCallback.deleter) {
        systemCallback.deleter();
    }
    instance = nullptr;
}
