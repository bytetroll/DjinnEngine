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
 on 6/28/2018.
//

#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Editor/EditorWorld.h>
#include <Game/IWorldHost.h>
#include <Game/IWorld.h>
#include <Game/IActorSystem.h>
#include <Base/DefaultLogs.h>
#include <Data/WorldConfig.h>
#include <Data/IAssetHost.h>
#include <UI/IUICanvas.h>
#include <UI/Elements/TransformDebug.h>
#include <Graphics/ScreenOutput.h>
#include <Graphics/IOutputHost.h>
#include <UI/IUIInputSystem.h>
#include <Editor/Controls/Bootstrap.h>

DJINN_NS2(Editor);

EditorWorld::EditorWorld(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IEditorWorld(registry, outer, host) {

}

COM::Result EditorWorld::Initialize() {
    // Get world worldHost
    Game::IWorldHost *host;
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected world worldHost to be installed");
        return COM::kOK;
    }

    // Get output worldHost
    Graphics::IOutputHost* outputHost;
    if (!registry->GetPipeline()->GetInterface(&outputHost)) {
        ErrorLog(this).Write("Expected output worldHost to be installed");
        return COM::kOK;
    }

    // Create base configuration
    Data::WorldConfig *config;
    if (!registry->CreateClass(this, &config)) {
        ErrorLog(this).Write("Failed to create config");
        return COM::kOK;
    }

    // Create and register world
    if (!host->CreateWorld(config, &world) || !host->Register(world)) {
        return COM::kOK;
    }

    // Get actor system
    Game::IActorSystem* actorSystem;
    if (!world->GetPipeline()->GetInterface(&actorSystem)) {
        ErrorLog(registry).Write("Expected actor system to be installed");
        return COM::kOK;
    }

    // Get actor system
    UI::IUIInputSystem* uiInputSystem;
    if (!world->GetPipeline()->GetInterface(&uiInputSystem)) {
        ErrorLog(registry).Write("Expected ui input system to be installed");
        return COM::kOK;
    }

    // Create canvas
    Check(world->CreateScene(nullptr, &canvas));

    // Configure canvas
    {
        // Attempt to create output
        Graphics::ScreenOutput *output;
        if (!registry->CreateClass(this, &output) || !output->Initialize() || !outputHost->Register(output) || !outputHost->SetOutput(output)) {
            ErrorLog(registry).Write("Failed to create output");
            return COM::kOK;
        }

        // Configure
        canvas->SetScaling(true);
        canvas->SetOutput(output);
        uiInputSystem->Register(canvas);
    }

    // Create bootstrap
    {
        Bootstrap* bootstrap;
        Check(actorSystem->CreateActor(canvas, &bootstrap));

        // Add
        canvas->AddElement(bootstrap);
    }

    /*{
        UI::TransformDebug *debug;
        Always(actorSystem->CreateActor(scene, &debug));
        canvas->AddElement(debug);
    }*/

    // OK
    return COM::kOK;
}

Game::IWorld *EditorWorld::GetGameWorld() {
    return world;
}

UI::IUICanvas *EditorWorld::GetGameCanvas() {
    return canvas;
}
