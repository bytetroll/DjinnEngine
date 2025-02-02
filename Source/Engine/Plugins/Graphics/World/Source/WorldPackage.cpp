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
