//
// Created by Dev on 8/30/2018.
//

#include <Game/Actors/DirectionalLight.h>
#include <Game/ISpatialScene.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Graphics/SceneDirectionalLightPacket.h>

DJINN_NS2(Game);

void DirectionalLight::Created() {
    if (auto scene = COM::Cast<ISpatialScene>(GetScene())) {
        // Register proxy
        scene->Register(this);

        // Always update
        SetDynamicPolicy(true);
    }
}

void DirectionalLight::BuildScene(SceneBuilder &bucket) {
    bucket.Add(SceneObject().SetIndex(0));
}

void DirectionalLight::Build(const Core::ArrayView<SceneObject> &requests, Graphics::SceneDirectionalLightPacketBuffer *buffer) {
    Graphics::SceneDirectionalLightPacket packet;
    packet.direction = GetTransform()->GetRotation().AsDirection();
    packet.color = color;
    packet.intensity = intensity;
    buffer->Add(packet);
}
