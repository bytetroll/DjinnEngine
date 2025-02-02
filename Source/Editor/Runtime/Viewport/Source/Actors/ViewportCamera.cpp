//
// Created by Dev on 7/13/2018.
//

#include <Editor/Actors/ViewportCamera.h>
#include <Base/IPipeline.h>
#include <Game/ActorComponents/TransformComponent.h>
#include <Game/ActorComponents/InputComponent.h>
#include <Game/Actors/Camera.h>
#include <Game/IActorSystem.h>

DJINN_NS2(Editor);

ViewportCamera::ViewportCamera(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IViewportCamera(registry, outer, host) {

}

void ViewportCamera::StaticInitialize() {
    SetTransform(CreateComponent<Game::TransformComponent>());

    Always(GetSystem()->CreateActor(GetScene(), &camera));
    camera->GetTransform()->SetParent(GetTransform());

    input = CreateComponent<Game::InputComponent>();
}

void ViewportCamera::Created() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    auto transform = GetTransform();

    input->BindMapping("CameraForward"_id, Game::InputState::eDown, [=] {
        direction += transform->GetForward();
    });

    input->BindMapping("CameraBackward"_id, Game::InputState::eDown, [=] {
        direction -= transform->GetForward();
    });

    input->BindMapping("CameraRight"_id, Game::InputState::eDown, [=] {
        direction += transform->GetRight();
    });

    input->BindMapping("CameraLeft"_id, Game::InputState::eDown, [=] {
        direction -= transform->GetRight();
    });

    input->BindMapping("CameraUp"_id, Game::InputState::eDown, [=] {
        direction -= transform->GetUp();
    });

    input->BindMapping("CameraDown"_id, Game::InputState::eDown, [=] {
        direction += transform->GetUp();
    });

    input->BindMapping("CameraCapture"_id, Game::InputState::eDown, [=] {
        isCaptured = true;
    });

    input->BindMapping("CameraCapture"_id, Game::InputState::eUp, [=] {
        isCaptured = false;
    });

    input->BindEvent(Game::InputEventType::eMouseMoved, [=](const Game::InputEvent &event) {
        if (isCaptured) {
            rotation = {event.mouseMoved.delta.x, event.mouseMoved.delta.y};
        }
    });
}

void ViewportCamera::Update(double delta) {
    auto transform = GetTransform();

    // Apply velocities
    positionVelocity += direction * delta * 0.2f;
    rotationVelocity += rotation * delta * 0.3f;

    // Apply rotation velocity
    angles += rotationVelocity;

    // Clamp y
    angles.y = Math::Clamp(angles.y, -Math::kPI / 2.0f, Math::kPI / 2.0f);

    // Unwind x
    if (angles.x > Math::kPI * 2) {
        angles.x -= Math::kPI * 2;
    } else if (angles.x < -Math::kPI * 2) {
        angles.x = Math::kPI * 2 - angles.x;
    }

    // Set transform
    transform->SetPosition(transform->GetPosition() + positionVelocity);
    transform->SetRotation(Math::Quaternion::RotateRPY({-angles.y, angles.x, 0}) * Math::Quaternion(transform->GetForward(), -rotationVelocity.x * 1.5f) * Math::Quaternion({1, 0, 0}, Math::kPI));

    // Damp velocities
    positionVelocity = Math::Lerp(positionVelocity, Math::Vector3(0, 0, 0), delta * 10.0f);
    rotationVelocity = Math::Lerp(rotationVelocity, Math::Vector2(0, 0), delta * 10.0f);

    // Reset immediates
    direction = {};
    rotation = {};
}

void ViewportCamera::SetOutput(Graphics::IOutput *output) {
    camera->SetOutput(output);
}

Game::ICamera *ViewportCamera::GetGameCamera() {
    return camera;
}
