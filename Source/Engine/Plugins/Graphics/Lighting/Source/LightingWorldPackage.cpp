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

#include <Graphics/LightingWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/ICameraProcess.h>
#include <Graphics/LightingProcess.h>
#include <Graphics/LightingPass.h>
#include <Graphics/IPipeline.h>

DJINN_NS2(Graphics);

LightingWorldPackage::LightingWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result LightingWorldPackage::Initialize(Game::IWorld *world) {
    // Get scene process
    ICameraProcess* cameraProcess;
    if (!world->GetPipeline()->GetInterface(&cameraProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, expected a world process to be installed");
        return COM::kError;
    }

    // Create camera process
    LightingProcess* lightingProcess;
    if (!registry->CreateClass(world, &lightingProcess) || !lightingProcess->Initialize() || !world->GetPipeline()->AddInterface(lightingProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, failed to install camera process");
        return COM::kError;
    }

    // Install camera pass
    LightingPass* pass;
    if (!registry->CreateClass(world, &pass) || !pass->Initialize(world, cameraProcess, lightingProcess) || !cameraProcess->GetPipeline()->Register(pass)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, failed to install camera pass");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result LightingWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result LightingWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
