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
 on 2/24/2018.
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

