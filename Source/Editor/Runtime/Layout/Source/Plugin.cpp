//
// Created by Dev on 6/27/2018.
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
