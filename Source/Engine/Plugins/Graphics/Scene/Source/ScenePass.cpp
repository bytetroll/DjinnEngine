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

