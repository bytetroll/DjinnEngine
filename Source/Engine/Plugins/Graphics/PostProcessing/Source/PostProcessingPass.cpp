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

