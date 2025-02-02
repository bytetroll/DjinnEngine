//
// Created by Dev on 10/11/2018.
//
#pragma once

#include <Game/ISceneActor.h>
#include <Game/ActorComponents/InputComponent.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Math/Vector3.h>
#include <Game/ActorCallback.h>

DJINN_NS();

class MoveComponent : public Game::ISceneComponent {
public:
    DJINN_COM_CLASS();

    MoveComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneComponent(registry, outer, host) {

    }

    void Created() override {
        callback.SetCallback(Bindff(Update));
        callback.Register(this);

        offset = GetSceneActor()->GetTransform()->GetPosition();
    }

    void Update(double delta) {
        GetSceneActor()->GetTransform()->SetPosition(offset + Math::Vector3(
            0.0f,
            Math::Cos(static_cast<float>((elapsed += delta) + offset.x + offset.z)) * 0.25f,
            0.0f
        ));
    }

private:
    Math::Vector3 offset;
    Game::ActorCallback callback;
    double elapsed = 0;
};