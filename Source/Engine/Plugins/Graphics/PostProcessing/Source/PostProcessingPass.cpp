//
// Created by Dev on 2/24/2018.
//

#include <Graphics/PostProcessingPass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/IProcess.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPostProcessingProcess.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/ICameraProcess.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/CameraPacket.h>
#include <Graphics/IGraphicsSystem.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <Game/IWorld.h>
#include <Graphics/FXAAUtil.h>

DJINN_NS2(Graphics);

PostProcessingPass::PostProcessingPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPostProcessingPass(registry, outer, host) {

}

COM::Result PostProcessingPass::Initialize(Game::IWorld *world, ICameraProcess *cameraProcess, IPostProcessingProcess *lightingProcess) {
    this->cameraProcess = cameraProcess;
    this->lightingProcess = lightingProcess;

    // Get system
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("PostProcessingDrawPass expected an installed graphics system");
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

COM::Result PostProcessingPass::Preprocess(PassBuilder builder) {
    // OK
    return COM::kOK;
}

COM::Result PostProcessingPass::Build(PassBuilder builder, double delta) {
    // Get packet
    auto packet = cameraProcess->GetPacket();

    // Resources
    PostProcessingResources resources;

    // Build process
    PassBuilder pass = builder.InlinedPass("PostProcessingprocess");
    if (!lightingProcess->Build(packet, resources, pass, delta)) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result PostProcessingPass::Configure() {
    // OK
    return COM::kOK;
}

