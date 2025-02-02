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
 on 3/1/2018.
//

#include <Graphics/DeferredLightingWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/ILightingProcess.h>
#include <Graphics/DeferredGBufferPass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/DeferredDirectionalLightPass.h>

DJINN_NS2(Graphics);

DeferredLightingWorldPackage::DeferredLightingWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result DeferredLightingWorldPackage::Initialize(Game::IWorld *world) {
    // Get lighting process
    ILightingProcess* lightingProcess;
    if (!world->GetPipeline()->GetInterface(&lightingProcess)) {
        ErrorLog(this).Write("Expected a world process to be installed");
        return COM::kError;
    }

    // Install gbuffer pass
    DeferredGBufferPass* gbufferPass;
    if (!registry->CreateClass(world, &gbufferPass) || !gbufferPass->Initialize(lightingProcess) || !lightingProcess->GetPipeline()->Register(gbufferPass)) {
        ErrorLog(this).Write("Failed to install gbuffer pass");
        return COM::kError;
    }

    // Install directional light pass
    DeferredDirectionalLightPass* directionalPass;
    if (!registry->CreateClass(world, &directionalPass) || !directionalPass->Initialize(lightingProcess) || !lightingProcess->GetPipeline()->Register(directionalPass)) {
        ErrorLog(this).Write("Failed to install directional light pass");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result DeferredLightingWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result DeferredLightingWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
