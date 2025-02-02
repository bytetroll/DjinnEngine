//
// Created by Dev on 2/27/2018.
//

#include <Graphics/WorldPass.h>
#include <Graphics/IWorldProcess.h>
#include <Graphics/WorldPacket.h>
#include <Graphics/PacketBatch.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/IPacketSystem.h>

DJINN_NS2(Graphics);

WorldPass::WorldPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldPass(registry, outer, host) {

}

COM::Result WorldPass::Initialize(Game::IWorld *world, IPacketSystem* packetSystem, IWorldProcess *worldProcess) {
    this->worldProcess = worldProcess;
    this->world = world;
    this->packetSystem = packetSystem;

    // Get graphics system
    if (!registry->GetPipeline()->GetInterface(&graphicsSystem)) {
        ErrorLog(this).Write("Failed to initialize world pass, expected a graphics system to be installed");
        return COM::kError;
    }

    return COM::kOK;
}

COM::Result WorldPass::Preprocess(PassBuilder builder) {
    return COM::kOK;
}

COM::Result WorldPass::Build(PassBuilder builder, double delta) {
    // Pool packet
    WorldPacket* packet = nullptr;
    if (packetSystem->PoolPacket(&packet)) {
        if (!worldProcess->Build(packet, builder, delta)) {
            packet->Release();
            return COM::kError;
        }

        // Release usage
        packet->Release();
    }

    // OK
    return COM::kOK;
}

COM::Result WorldPass::Configure() {
    return COM::kOK;
}

