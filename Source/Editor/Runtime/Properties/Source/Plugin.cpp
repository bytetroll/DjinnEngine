//
// Created by Dev on 6/27/2018.
//

#include <Host/PluginAPI.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Editor/IPropertiesHost.h>
#include <Editor/WorldFeatures.h>
#include <Game/IWorldHost.h>
#include <Game/IWorldFeature.h>
#include <Editor/PropertiesWorldPackage.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_PROPERTIES COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("Expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(kEditorFeature)->AddPackage<PropertiesWorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_PROPERTIES COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
