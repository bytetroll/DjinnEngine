//
// Created by Dev on 3/1/2018.
//

#include <UI/DesktopUIWorldPackage.h>
#include <Game/IGameSystem.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Game/IEntitySystem.h>

DJINN_NS2(UI);

DesktopUIWorldPackage::DesktopUIWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result DesktopUIWorldPackage::Initialize(Game::IWorld *world) {
    // Get entity system
    Game::IEntitySystem *entitySystem;
    if (!world->GetPipeline()->GetInterface(&entitySystem)) {
        ErrorLog(this).Write("Expected an installed entity system");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result DesktopUIWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result DesktopUIWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
