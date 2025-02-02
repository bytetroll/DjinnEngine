//
// Created by Dev on 2/24/2018.
//

#include <Graphics/UIPass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/IProcess.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IOutput.h>
#include <Graphics/IUIProcess.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/PacketBatch.h>
#include <Graphics/UIPacket.h>
#include <Graphics/IGraphicsSystem.h>
#include <GHA/ISwapchain.h>
#include <GHA/ITexture.h>
#include <Game/IWorld.h>
#include <Graphics/FXAAUtil.h>
#include <Graphics/WorldProcess.h>
#include <Graphics/WorldPacket.h>

DJINN_NS2(Graphics);

UIPass::UIPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIPass(registry, outer, host) {

}

COM::Result UIPass::Initialize(Game::IWorld *world, IWorldProcess *worldProcess, IUIProcess *uiProcess) {
    this->worldProcess = worldProcess;
    this->uiProcess = uiProcess;

    // Get system
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("UIDrawPass expected an installed graphics system");
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

COM::Result UIPass::Preprocess(PassBuilder builder) {
    // OK
    return COM::kOK;
}

COM::Result UIPass::Build(PassBuilder builder, double delta) {
    // Get packet
    auto packet = worldProcess->GetPacket()->batch.GetPacket<UIPacketBuffer>();

    // Build packets
    for (auto &pck : packet->GetPackets()) {
        if (!pck->output) {
            // TODO: Logging?
            continue;
        }

        // Depth texture
        Texture depthTexture = builder.AddTexture(
                GHA::TextureDesc()
                        .Format(GHA::Format::eD32F)
                        .Usage(GHA::TextureUsage::eDepthStencil)
                        .Size(pck->output->GetTexture(builder)->size)
                        .Flags(GHA::TextureFlag::eClearable)
        );

        // Clear
        builder.ClearColor(pck->output->GetTexture(builder));
        builder.ClearDepthStencil(depthTexture);

        // Build process
        PassBuilder pass = builder.InlinedPass("uiprocess");
        if (!uiProcess->Build(pck, depthTexture, pass, delta)) {
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result UIPass::Configure() {
    // OK
    return COM::kOK;
}

