//
// Created by Dev on 3/1/2018.
//

#include <Graphics/PacketSystem.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/PacketBatch.h>
#include <GHA/ISwapchain.h>
#include <Host/IPipeline.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPacketBatcher.h>

DJINN_NS2(Graphics);

PacketSystem::PacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPacketSystem(registry, outer, host) {

}

COM::Result PacketSystem::Initialize() {
    // Get graphics system
    if (!registry->GetPipeline()->GetInterface(&graphicsSystem)) {
        ErrorLog(this).Write("Failed to initialize world pass, expected a graphics system to be installed");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result PacketSystem::Run(Game::StepMode mode, double delta) {
    // Prevent pooling too many packets
    if (Platform::LockGuard guard(packetMtx); packetQueue.Size() >= 10) {
        return COM::kOK;
    }

    // Pop packet
    WorldPacket* packet;
    {
        Platform::LockGuard guard(packetMtx);
        packet = packets.Pop();
    }
    packet->Flush();

    packet->deleter = [this, packet] {
        Platform::LockGuard guard(packetMtx);
        this->packets.Push(packet);
    };

    // Run all batchers
    Async::Group group;
    for (auto batcher : batchers) {
        batcher->BuildAsync(group, packet);
    }

    // Submit
    group.Wait();
    {
        Platform::LockGuard guard(packetMtx);
        packetQueue.Add(packet);
    }

    // OK
    return COM::kOK;
}

COM::Result PacketSystem::Register(IPacketBatcher *batcher) {
    batchers.Add(batcher);
    return COM::kOK;
}

COM::Result PacketSystem::Deregister(IPacketBatcher *batcher) {
    batchers.RemoveValue(batcher);
    return COM::kOK;
}

bool PacketSystem::PoolPacket(WorldPacket **out) {
    Platform::LockGuard guard(packetMtx);
    return packetQueue.Pop(*out);
}

Game::StepModeSet PacketSystem::GetStepMode() {
    return Game::StepMode::eOnce;
}

COM::Result PacketSystem::Configure(Base::PipelineBuilder& builder) {
    return COM::kOK;
}
