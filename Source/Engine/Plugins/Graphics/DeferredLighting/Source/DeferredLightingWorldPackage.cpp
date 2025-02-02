//
// Created by Dev on 3/1/2018.
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
