//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorldHost.h>
#include <Game/IWorldFeature.h>
#include <UI/UIMLWorldPackage.h>
#include <UI/UIMLHost.h>

DJINN_NS();

namespace {
}

DJINN_C_EXPORT_PLUGINS_UI_UIML COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("UIML plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<UI::UIMLWorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_UI_UIML COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
