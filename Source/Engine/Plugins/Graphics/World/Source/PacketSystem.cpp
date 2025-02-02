//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 3/1/2018.
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
