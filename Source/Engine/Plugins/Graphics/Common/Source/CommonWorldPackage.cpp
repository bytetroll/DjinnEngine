//
// Created by Dev on 3/1/2018.
//

#include <Graphics/CommonWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/IPipeline.h>
#include <Graphics/FXAAUtil.h>

DJINN_NS2(Graphics);

CommonWorldPackage::CommonWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result CommonWorldPackage::Initialize(Game::IWorld *world) {
    // Install FXAA
    FXAAUtil* fxaa;
    if (!registry->CreateClass(world, &fxaa) || !fxaa->Initialize() || !world->GetPipeline()->AddClass(fxaa)) {
        ErrorLog(this).Write("Failed to initialize Common graphics plugin, failed to install Common pass");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result CommonWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result CommonWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
