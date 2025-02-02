//
// Created by Dev on 3/1/2018.
//

#include <Graphics/WorldPackage.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/WorldProcess.h>
#include <Graphics/WorldPass.h>
#include <Graphics/IGraph.h>
#include <Graphics/IProcess.h>
#include <Graphics/IPipeline.h>
#include <Graphics/PacketSystem.h>

DJINN_NS2(Graphics);

WorldPackage::WorldPackage(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPackage(registry, outer, host) {

}

COM::Result WorldPackage::Initialize(Game::IWorld *world) {
    // Get graphics system
    IGraphicsSystem *system;
    if (!registry->GetPipeline()->GetInterface(&system)) {
        ErrorLog(this).Write("Failed to initialize world graphics plugin, expected graphics system to be installed");
        return COM::kError;
    }

    // Get process
    auto process = system->GetGraph()->GetProcess();

    // Create world process
    WorldProcess *worldProcess;
    if (!registry->CreateClass(world, &worldProcess) || !worldProcess->Initialize() || !world->GetPipeline()->AddInterface(worldProcess)) {
        ErrorLog(this).Write("Failed to initialize world graphics plugin, failed to install world process");
        return COM::kError;
    }

    // Install packet system
    PacketSystem *packetSystem;
    if (!registry->CreateClass(world, &packetSystem) || !packetSystem->Initialize() || !world->Register(packetSystem) || !world->GetPipeline()->AddInterface(packetSystem)) {
        ErrorLog(this).Write("Failed to initialize world graphics plugin, failed to install world pass");
        return COM::kError;
    }

    // Install world pass
    WorldPass *pass;
    if (!registry->CreateClass(world, &pass) || !pass->Initialize(world, packetSystem, worldProcess) || !process->GetPipeline()->Register(pass)) {
        ErrorLog(this).Write("Failed to initialize world graphics plugin, failed to install world pass");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}


COM::Result WorldPackage::Serialize(Serialization::Archive &archive) {
    return COM::kOK;
}

COM::Result WorldPackage::Deserialize(Serialization::Archive &archive) {
    return COM::kOK;
}
