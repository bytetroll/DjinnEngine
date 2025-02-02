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
 on 2/27/2018.
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

