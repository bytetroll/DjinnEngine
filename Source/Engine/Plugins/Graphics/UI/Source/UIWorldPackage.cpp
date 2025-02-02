//
// Created by Dev on 3/1/2018.
//

#include <Graphics/UIWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/ISceneProcess.h>
#include <Graphics/UIProcess.h>
#include <Graphics/UIPass.h>
#include <Graphics/IPipeline.h>
#include <Graphics/UIDrawPass.h>
#include <Graphics/IWorldProcess.h>

DJINN_NS2(Graphics);

UIWorldPackage::UIWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result UIWorldPackage::Initialize(Game::IWorld *world) {
    // Get world process
    IWorldProcess* worldProcess;
    if (!world->GetPipeline()->GetInterface(&worldProcess)) {
        ErrorLog(this).Write("Failed to initialize camera graphics plugin, expected a world process to be installed");
        return COM::kError;
    }

    // Create ui process
    UIProcess* uiProcess;
    if (!registry->CreateClass(world, &uiProcess) || !uiProcess->Initialize()) {
        ErrorLog(this).Write("Failed to initialize ui graphics plugin, failed to install ui process");
        return COM::kError;
    }

    // Install ui pass
    UIPass* pass;
    if (!registry->CreateClass(world, &pass) || !pass->Initialize(world, worldProcess, uiProcess) || !worldProcess->GetPipeline()->Register(pass)) {
        ErrorLog(this).Write("Failed to initialize ui graphics plugin, failed to install ui pass");
        return COM::kError;
    }

    // Create ui draw pass
    UIDrawPass* drawPass;
    if (!registry->CreateClass(world, &drawPass) || !drawPass->Initialize(uiProcess) || !uiProcess->GetPipeline()->Register(drawPass)) {
        ErrorLog(this).Write("Failed to initialize ui graphics plugin, failed to install ui draw pass into ui process");
        return COM::kError;
    }
    
    // OK
    return COM::kOK;
}


COM::Result UIWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result UIWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
