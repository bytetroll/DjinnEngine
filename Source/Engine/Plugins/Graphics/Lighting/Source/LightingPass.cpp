//
// Created by Dev on 2/24/2018.
//

#include <Graphics/LightingPass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/IProcess.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/ILightingProcess.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/ICameraProcess.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/CameraPacket.h>
#include <Graphics/IGraphicsSystem.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <Game/IWorld.h>
#include <Graphics/FXAAUtil.h>
#include <Graphics/IOutput.h>

DJINN_NS2(Graphics);

LightingPass::LightingPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILightingPass(registry, outer, host) {

}

COM::Result LightingPass::Initialize(Game::IWorld *world, ICameraProcess *cameraProcess, ILightingProcess *lightingProcess) {
    this->cameraProcess = cameraProcess;
    this->lightingProcess = lightingProcess;

    // Get system
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("Expected an installed graphics system");
        return COM::kOK;
    }

    // Get fxaa
    if (!world->GetPipeline()->GetClass(&fxaa)) {
        ErrorLog(this).Write("Expected an installed fxaa util");
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

COM::Result LightingPass::Preprocess(PassBuilder builder) {
    // OK
    return COM::kOK;
}

COM::Result LightingPass::Build(PassBuilder builder, double delta) {
    // Get packet
    auto packet = cameraProcess->GetPacket();
    if (!packet->output) {
        // TODO: Logging of sorts?
        return COM::kOK;
    }

    // Resources
    LightingResources resources;

    // Direct texture
    resources.direct = builder.AddTexture(
            GHA::TextureDesc()
                    .Format(GHA::Format::eR16G16B16A16F)
                    .Usage(GHA::TextureUsage::eColor)
                    .Size(packet->output->GetTexture(builder)->size)
                    .Flags(GHA::TextureFlag::eClearable)
    );

    // Clear
    builder.ClearColor(resources.direct);

    // Build process
    PassBuilder pass = builder.InlinedPass("Lightingprocess");
    if (!lightingProcess->Build(packet, resources, pass, delta)) {
        return COM::kError;
    }

    // Post resolve
    {
        PassBuilder resolve = pass.Pass("lightingresolve");
        resolve.AddDependency(pass);

        // Copy to backbuffer
        Check(fxaa->Build(resolve, resources.direct, packet->output->GetTexture(builder)));
    }

    // OK
    return COM::kOK;
}

COM::Result LightingPass::Configure() {
    // OK
    return COM::kOK;
}

