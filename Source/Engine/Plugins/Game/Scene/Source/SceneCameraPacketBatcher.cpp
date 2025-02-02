//
// Created by Dev on 8/30/2018.
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
