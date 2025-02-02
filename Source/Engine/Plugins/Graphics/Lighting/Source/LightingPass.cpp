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

