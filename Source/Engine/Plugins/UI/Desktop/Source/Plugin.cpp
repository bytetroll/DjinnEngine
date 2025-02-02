//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Game/IEntitySystem.h>
#include <UI/DesktopUIWorldPackage.h>
#include <Game/IWorldHost.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

namespace {
    //Game::InputConnection escapeConnection;
}

DJINN_C_EXPORT_PLUGINS_UI_DESKTOP COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("Desktop UI plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<UI::DesktopUIWorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_UI_DESKTOP COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
