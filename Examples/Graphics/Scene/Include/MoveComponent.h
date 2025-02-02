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
    }

    void Update(double delta) {
        GetSceneActor()->GetTransform()->SetRotation(Math::Quaternion::RotateRPY({0, 0, static_cast<float>(Math::Cos(elapsed += delta)) * 0.1f}));
    }

private:
    Game::ActorCallback callback;
    double elapsed = 0;
};