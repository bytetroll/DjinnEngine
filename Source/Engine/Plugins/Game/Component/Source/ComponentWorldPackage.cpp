//
// Created by Dev on 3/1/2018.
//

#include <Game/ComponentWorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/EntitySystem.h>
#include <Game/ActorSystem.h>
#include <Game/IWorld.h>

DJINN_NS2(Game);

ComponentWorldPackage::ComponentWorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result ComponentWorldPackage::Initialize(IWorld *world) {
    // Install entity system
    EntitySystem *entitySystem;
    if (!registry->CreateClass(world, &entitySystem) || !entitySystem->Initialize() || !world->GetPipeline()->AddInterface(entitySystem) || !world->Register(entitySystem)) {
        ErrorLog(this).Write("Failed to install entity system into game system");
        return COM::kError;
    }

    // Install actor system
    ActorSystem *actorSystem;
    if (!registry->CreateClass(world, &actorSystem) || !world->GetPipeline()->AddInterface(actorSystem) || !world->Register(actorSystem)) {
        ErrorLog(this).Write("Failed to install actor system into game system");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result ComponentWorldPackage::Serialize(Serialization::Archive &archive) {
    // OK
    return COM::kOK;
}

COM::Result ComponentWorldPackage::Deserialize(Serialization::Archive &archive) {
    // OK
    return COM::kOK;
}
