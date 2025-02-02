//
// Created by Dev on 3/1/2018.
//

#include <Graphics/CameraWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/CameraProcess.h>
#include <Graphics/CameraPass.h>
#include <Graphics/IPipeline.h>

DJINN_NS2(Graphics);

CameraWorldPackage::CameraWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result CameraWorldPackage::Initialize(Game::IWorld *world) {
    // Get scene process
    ISceneProcess* sceneProcess;
    if (!world->GetPipeline()->GetInterface(&sceneProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, expected a world process to be installed");
        return COM::kError;
    }

    // Create camera process
    CameraProcess* cameraProcess;
    if (!registry->CreateClass(world, &cameraProcess) || !cameraProcess->Initialize() || !world->GetPipeline()->AddInterface(cameraProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, failed to install camera process");
        return COM::kError;
    }

    // Install camera pass
    CameraPass* pass;
    if (!registry->CreateClass(world, &pass) || !pass->Initialize(sceneProcess, cameraProcess) || !sceneProcess->GetPipeline()->Register(pass)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, failed to install camera pass");
        return COM::kError;
    }
    
    // OK
    return COM::kOK;
}


COM::Result CameraWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result CameraWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
