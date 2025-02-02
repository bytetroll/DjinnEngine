//
// Created by Dev on 6/27/2018.
//

#include <Host/PluginAPI.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Editor/IEditorWorld.h>
#include <Game/IWorld.h>
#include <Data/IAssetHost.h>
#include <Editor/LevelHost.h>
#include <Editor/ComponentHost.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_LEVEL COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Create component worldHost
        ComponentHost *comHost;
        if (!registry->CreateClass(plugin, &comHost) || !registry->GetPipeline()->AddInterface(comHost)) {
            ErrorLog(registry).Write("Failed to create component worldHost");
            return COM::kError;
        }

        // Create scene world worldHost
        LevelHost *levelHost;
        if (!registry->CreateClass(plugin, &levelHost) || !levelHost->Initialize() || !registry->GetPipeline()->AddInterface(levelHost)) {
            ErrorLog(registry).Write("Failed to create world worldHost");
            return COM::kError;
        }

        // Get editor world
        IEditorWorld *world;
        if (!registry->GetPipeline()->GetInterface(&world)) {
            ErrorLog(registry).Write("Expected editor world to be installed");
            return COM::kOK;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_LEVEL COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
