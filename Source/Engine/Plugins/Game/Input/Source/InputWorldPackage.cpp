//
// Created by Dev on 3/1/2018.
//

#include <Game/InputWorldPackage.h>
#include <Game/IGameSystem.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/EntitySystem.h>
#include <Game/IWorld.h>
#include <Game/InputSystem.h>
#include <Game/WindowInputDevice.h>
#include <Base/IWindowSystem.h>

DJINN_NS2(Game);

InputWorldPackage::InputWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result InputWorldPackage::Initialize(IWorld *world) {
    // Get entity system
    IEntitySystem* entitySystem;
    if (!world->GetPipeline()->GetInterface(&entitySystem)) {
        ErrorLog(this).Write("Input plugin expected an installed entity system");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result InputWorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result InputWorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
