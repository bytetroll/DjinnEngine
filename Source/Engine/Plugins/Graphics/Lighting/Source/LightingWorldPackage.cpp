//
// Created by Dev on 3/1/2018.
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
