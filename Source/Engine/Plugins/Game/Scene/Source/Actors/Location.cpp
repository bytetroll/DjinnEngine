//
// Created by Dev on 11/15/2018.
//

#include <Game/Actors/Location.h>
#include <Game/ActorComponents/TransformComponent.h>

DJINN_NS2(Game);

Location::Location(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : ILocation(registry, outer, host) {

}

void Location::StaticInitialize() {
    SetTransform(transform = CreateComponent<TransformComponent>());

    callback.SetCallback(Bindff(Update));
    callback.Register(this);
}

void Location::AddActor(ISceneActor *actor) {
    actor->GetTransform()->SetParent(transform);
}

void Location::RemoveActor(ISceneActor *actor) {
    actor->GetTransform()->SetParent(nullptr);
}

void Location::Update(double delta) {
    transform->UpdateTransform();
}
