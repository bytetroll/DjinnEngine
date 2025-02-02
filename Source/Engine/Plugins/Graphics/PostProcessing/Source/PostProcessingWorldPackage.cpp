//
// Created by Dev on 3/1/2018.
//

#include <Graphics/PostProcessingWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/ICameraProcess.h>
#include <Graphics/PostProcessingProcess.h>
#include <Graphics/PostProcessingPass.h>
#include <Graphics/IPipeline.h>

DJINN_NS2(Graphics);

PostProcessingWorldPackage::PostProcessingWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result PostProcessingWorldPackage::Initialize(Game::IWorld *world) {
    // Get scene process
    ICameraProcess *cameraProcess;
    if (!world->GetPipeline()->GetInterface(&cameraProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, expected a world process to be installed");
        return COM::kError;
    }

    // Create camera process
    PostProcessingProcess *ppProcess;
    if (!registry->CreateClass(world, &ppProcess) || !ppProcess->Initialize() || !world->GetPipeline()->AddInterface(ppProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, failed to install camera process");
        return COM::kError;
    }

    // Install camera pass
    PostProcessingPass *pass;
    if (!registry->CreateClass(world, &pass) || !pass->Initialize(world, cameraProcess, ppProcess) || !cameraProcess->GetPipeline()->Register(pass)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, failed to install camera pass");
        return COM::kError;
    }
    
    // OK
    return COM::kOK;
}


COM::Result PostProcessingWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result PostProcessingWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
