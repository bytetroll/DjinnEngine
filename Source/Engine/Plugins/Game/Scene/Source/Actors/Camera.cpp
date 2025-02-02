//
// Created by Dev on 8/30/2018.
//

#include <Game/Actors/Camera.h>
#include <Game/ICameraHost.h>
#include <Game/IWorld.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Graphics/CameraPacket.h>
#include <Graphics/IOutput.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Game);

Camera::Camera(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneActor(registry, outer, host) {

}

void Camera::Created() {
    ICameraHost* host;
    CheckMsgVoid(world->GetPipeline()->GetInterface(&host), Error, "Expected camera worldHost to be installed");

    // Register this
    CheckVoid(host->Register(this));
}

void Camera::Build(Graphics::CameraPacket *packet) {
    // Must have output
    if (!output) {
        return;
    }

    // Get output size
    auto outputSize = output->GetSize();

    // Set output
    packet->output = output;
    packet->view = GetTransform()->GetWorldTransform().Inversed();
    packet->projection = Math::Matrix4::Perspective(Math::kPI / 4.0f, static_cast<float>(outputSize.width) / static_cast<float>(outputSize.height), 0.1f, 1000.0f);
}

void Camera::SetDirection(const Math::Vector3 &direction) {
    GetTransform()->SetRotation(Math::Matrix4::Lookat({-0.000001f, 0, +0.000001f}, direction).Inversed().Decompose().rotation);
}

void Camera::Lookat(const Math::Vector3 &eye, const Math::Vector3 &focus) {
    GetTransform()->SetPosition(eye);
    SetDirection(focus - eye);
}

Graphics::IOutput *Camera::GetOutput() {
    return output;
}

