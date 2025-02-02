//
// Created by Dev on 3/1/2018.
//

#include <UI/DiagnosticUIWorldPackage.h>
#include <Game/IGameSystem.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Game/IEntitySystem.h>

DJINN_NS2(UI);

DiagnosticUIWorldPackage::DiagnosticUIWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result DiagnosticUIWorldPackage::Initialize(Game::IWorld *world) {
    // Get entity system
    Game::IEntitySystem *entitySystem;
    if (!world->GetPipeline()->GetInterface(&entitySystem)) {
        ErrorLog(this).Write("UI plugin expected an installed entity system");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result DiagnosticUIWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result DiagnosticUIWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
