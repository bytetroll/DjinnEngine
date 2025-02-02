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
 on 6/27/2018.
//

#include <Host/PluginAPI.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Editor/IEditorWorld.h>
#include <Game/IActorSystem.h>
#include <Game/IWorld.h>
#include <UI/IUICanvas.h>
#include <Editor/Elements/MainLayout.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_LAYOUT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get editor world
        IEditorWorld* world;
        if (!registry->GetPipeline()->GetInterface(&world)) {
            ErrorLog(registry).Write("Expected editor world to be installed");
            return COM::kOK;
        }

        // Get actor system
        Game::IActorSystem* actorSystem;
        if (!world->GetGameWorld()->GetPipeline()->GetInterface(&actorSystem)) {
            ErrorLog(registry).Write("Expected entity system to be installed");
            return COM::kOK;
        }

        // Attempt to create main layout
        MainLayout* layout;
        {
            if (!actorSystem->CreateActor(world->GetGameCanvas(), &layout) || !registry->GetPipeline()->AddInterface(layout)) {
                ErrorLog(registry).Write("Failed to create editor layout");
                return COM::kError;
            }

            // Add layout
            world->GetGameCanvas()->AddElement(layout);
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_LAYOUT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
