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

#include <Game/SceneCameraPacketBatcher.h>
#include <Game/ICameraHost.h>
#include <Game/ICamera.h>
#include <Graphics/ScenePacket.h>
#include <Graphics/CameraPacket.h>
#include <Game/ISpatialScene.h>
#include <Game/CameraRenderer.h>

DJINN_NS2(Game);

SceneCameraPacketBatcher::SceneCameraPacketBatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneCameraPacketBatcher(registry, outer, host) {

}

COM::Result SceneCameraPacketBatcher::Initialize(ICameraHost *host) {
    this->host = host;

    // OK
    return COM::kOK;
}

COM::Result SceneCameraPacketBatcher::Build(Game::IScene *scene, Graphics::ScenePacket *packet) {
    auto spatialScene = COM::Cast<Game::ISpatialScene>(scene);
    if (!spatialScene) {
        return COM::kOK;
    }

    // Get packet buffer
    auto cameraBuffer = packet->batch.GetPacket<Graphics::CameraPacketBuffer>();

    // Process cameras
    for (auto camera : host->GetCameras()) {
        // Add camera packet
        auto cameraPacket = cameraBuffer->Add();

        // Build camera
        camera->Build(cameraPacket);

        // SpatialScene renderer
        CameraRenderer renderer;
        renderer.packet = cameraPacket;

        // Build scene viewport
        Check(spatialScene->BuildViewport(&renderer, cameraPacket->projection * cameraPacket->view));
    }

    // OK
    return COM::kOK;
}
