//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/30/2018.
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

