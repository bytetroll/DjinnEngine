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
