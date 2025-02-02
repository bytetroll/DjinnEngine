//
// Created by Dev on 3/1/2018.
//

#include <UI/UIMLWorldPackage.h>
#include <Game/IGameSystem.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Game/IEntitySystem.h>
#include <UI/UIMLHost.h>

DJINN_NS2(UI);

UIMLWorldPackage::UIMLWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result UIMLWorldPackage::Initialize(Game::IWorld *world) {
    // Install UIML worldHost
    UIMLHost *uimlHost;
    if (!registry->CreateClass(world, &uimlHost) || !uimlHost->Initialize() || !world->GetPipeline()->AddInterface(uimlHost)) {
        ErrorLog(this).Write("Failed to install UIML worldHost");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result UIMLWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result UIMLWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
