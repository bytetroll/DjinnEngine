//
// Created by Dev on 8/15/2018.
//

#include <Game/IActorComponent.h>
#include <Game/IActorSystem.h>

DJINN_NS2(Game);

IActorComponent::IActorComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActorSystemInstance(registry, outer, host) {

}

void IActorComponent::Destroy() {
    Sink(GetSystem()->DestroyComponent(this));
    isDeleted = true;
}

COM::Result IActorComponent::Released() {
    Destroyed();
    return COM::kOK;
}
