//
// Created by Dev on 8/30/2018.
//

#include <Game/ActorComponents/TransformComponent.h>

DJINN_NS2(Game);

TransformComponent::TransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITransformComponent(registry, outer, host) {

}

void TransformComponent::Update() {
    // Calculate local transform
    auto localTransform = userTransform * (Math::Matrix4::Scale(scale) * Math::Matrix4::Rotate(rotation) * Math::Matrix4::Translate(position));

    // Parent offset
    if (parent) {
        worldTransform = parent->GetWorldTransform() * localTransform;
    } else {
        worldTransform = localTransform;
    }
}
