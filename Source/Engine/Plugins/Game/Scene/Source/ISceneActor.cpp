//
// Created by Dev on 8/30/2018.
//

#include <Game/ISceneActor.h>
#include <Game/ActorComponents/TransformComponent.h>

DJINN_NS2(Game);

void ISceneActor::StaticInitialize() {
    // Default transform
    SetTransform(CreateComponent<TransformComponent>());
}

void ISceneActor::SetTransform(ITransformComponent *transform) {
    this->transform = transform->GetWeakPtr();
}

