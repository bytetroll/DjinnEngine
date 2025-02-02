//
// Created by Dev on 10/11/2018.
//
#pragma once

#include <Game/ISceneActor.h>
#include <Game/ActorComponents/InputComponent.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Math/Vector3.h>
#include <Game/ActorCallback.h>
#include <Game/Actors/Camera.h>
#include <Game/ActorComponents/TransformComponent.h>
#include <Game/Actors/Camera.h>
#include <Game/IActorSystem.h>

DJINN_NS();

class EditorCamera : public Game::ISceneActor {
public:
    DJINN_COM_CLASS();

    EditorCamera(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneActor(registry, outer, host) {

    }

    void StaticInitialize() override {
        SetTransform(CreateComponent<Game::TransformComponent>());

        Always(GetSystem()->CreateActor(GetScene(), &camera));
        camera->GetTransform()->SetParent(GetTransform());

        input = CreateComponent<Game::InputComponent>();
    }

    void Created() override;

    void Update(double delta);

    void SetOutput(Graphics::IOutput *output) {
        camera->SetOutput(output);
    }

private:
    Game::InputComponent *input = nullptr;
    Game::Camera *camera;
    Game::ActorCallback callback;

    /// Immediate
    Math::Vector3 direction;
    Math::Vector2 rotation;

    /// Velocities
    Math::Vector3 positionVelocity;
    Math::Vector2 rotationVelocity;

    /// Current camera angles
    Math::Vector2 angles;

    /// Is this camera captured?
    bool isCaptured = false;
};
