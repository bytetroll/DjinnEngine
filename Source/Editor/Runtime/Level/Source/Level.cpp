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
 on 7/11/2018.
//

#include <Host/IRegistry.h>
#include <Host/Pipeline.h>
#include <Editor/Level.h>
#include <Editor/CommandHost.h>
#include <Base/DefaultLogs.h>
#include <Base/IPipeline.h>
#include <Game/IWorldHost.h>
#include <Game/IWorld.h>
#include <Game/IEntitySystem.h>
#include <Data/WorldConfig.h>
#include "../../Layout/Include/Editor/WorldFeatures.h"
#include <Game/IActorSystem.h>
#include <Editor/CommandHost.h>
#include <Editor/IEditorWorld.h>

DJINN_NS2(Editor);

Level::Level(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILevel(registry, outer, host) {

}

COM::Result Level::Initialize() {
    // Create pipeline
    pipeline = new Host::Pipeline();

    // Get world worldHost
    Game::IWorldHost *worldHost;
    if (!registry->GetPipeline()->GetInterface(&worldHost)) {
        ErrorLog(this).Write("Expected world worldHost to be installed");
        return COM::kOK;
    }

    // Create base configuration
    Data::WorldConfig *config;
    if (!registry->CreateClass(this, &config)) {
        ErrorLog(this).Write("Failed to create config");
        return COM::kOK;
    }

    // Add features
    config->AddFeature(kEditorFeature);

    // Create and register world
    if (!worldHost->CreateWorld(config, &gameWorld) || !worldHost->Register(gameWorld)) {
        return COM::kOK;
    }

    // Get editor world
    IEditorWorld *parentEditorWorld;
    if (!registry->GetPipeline()->GetInterface(&parentEditorWorld)) {
        ErrorLog(registry).Write("Expected editor world to be installed");
        return COM::kOK;
    }

    // Game worlds must run in serial with the editor world
    // The UI has several inter-world dependencies
    Check(worldHost->GetPipeline()->AddDependency(gameWorld, parentEditorWorld->GetGameWorld()));

    // Add reference to this level in the pipeline
    Check(gameWorld->GetPipeline()->AddInterface(this));

    // Create command worldHost
    if (!registry->CreateClass(this, &commandHost)) {
        ErrorLog(this).Write("Failed to create command worldHost");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

Game::IWorld *Level::GetGameWorld() {
    return gameWorld;
}

ICommandHost *Level::GetCommandHost() {
    return commandHost;
}

Host::IPipeline *Level::GetPipeline() {
    return pipeline;
}
