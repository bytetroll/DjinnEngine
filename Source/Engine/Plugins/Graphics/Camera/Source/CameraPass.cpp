//
// Created by Dev on 2/27/2018.
//

#include <Graphics/CameraPass.h>
#include <Graphics/ICameraProcess.h>
#include <Graphics/CameraPacket.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/ScenePacket.h>

DJINN_NS2(Graphics);

CameraPass::CameraPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ICameraPass(registry, outer, host) {

}

COM::Result CameraPass::Initialize(ISceneProcess* sceneProcess, ICameraProcess *cameraProcess) {
    this->cameraProcess = cameraProcess;
    this->sceneProcess = sceneProcess;
    return COM::kOK;
}

COM::Result CameraPass::Preprocess(PassBuilder builder) {
    return COM::kOK;
}

COM::Result CameraPass::Build(PassBuilder builder, double delta) {
    // Get buffer
    auto buffer = sceneProcess->GetPacket()->batch.GetPacket<CameraPacketBuffer>();

    // Process packets
    for (const auto &packet : buffer->GetPackets()) {
        if (!cameraProcess->Build(packet, builder, delta)) {
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result CameraPass::Configure() {
    return COM::kOK;
}

