//
// Created by Dev on 2/24/2018.
//

#include <Graphics/ScenePass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/IProcess.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/IGraphicsSystem.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <Game/IWorld.h>
#include <Graphics/FXAAUtil.h>
#include <Graphics/ScenePacket.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/IWorldProcess.h>
#include <Graphics/WorldPacket.h>

DJINN_NS2(Graphics);

ScenePass::ScenePass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IScenePass(registry, outer, host) {

}

COM::Result ScenePass::Initialize(IWorldProcess* worldProcess, ISceneProcess *uiProcess) {
    this->sceneProcess = uiProcess;
    this->worldProcess = worldProcess;

    // Get system
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("UIDrawPass expected an installed graphics system");
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

COM::Result ScenePass::Preprocess(PassBuilder builder) {
    // OK
    return COM::kOK;
}

COM::Result ScenePass::Build(PassBuilder builder, double delta) {
    // Get buffer
    auto buffer = worldProcess->GetPacket()->batch.GetPacket<ScenePacketBuffer>();

    // Process packets
    for (const auto &packet : buffer->GetPackets()) {
        // Build packet
        PassBuilder pass = builder.InlinedPass("sceneprocess");
        if (!sceneProcess->Build(packet, pass, delta)) {
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result ScenePass::Configure() {
    // OK
    return COM::kOK;
}

