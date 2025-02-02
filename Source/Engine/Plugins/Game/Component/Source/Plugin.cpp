//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Game/EntitySystem.h>
#include <Game/IGameSystem.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorldHost.h>
#include <Game/ComponentWorldPackage.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GAME_COMPONENT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("Component plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<Game::ComponentWorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GAME_COMPONENT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
