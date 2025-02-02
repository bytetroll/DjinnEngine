//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Game/World.h>
#include <Game/WorldHost.h>
#include <Game/WorldHostSystem.h>
#include <Game/IGameSystem.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GAME_WORLD COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get game system
        Game::IGameSystem* gameSystem;
        if (!registry->GetPipeline()->GetInterface(&gameSystem)) {
            ErrorLog(registry).Write("World plugin expected an installed game system");
            return COM::kError;
        }

        // Install worldHost
        Game::WorldHost* host;
        if (!registry->CreateClass(plugin, &host) || !host->Initialize() || !registry->GetPipeline()->AddInterface(host)) {
            ErrorLog(registry).Write("Failed to install world worldHost");
            return COM::kError;
        }

        // Install system
        Game::WorldHostSystem* system;
        if (!registry->CreateClass(plugin, &system) || !system->Initialize(host) || !registry->GetPipeline()->AddInterface(system) || !gameSystem->Register(system)) {
            ErrorLog(registry).Write("Failed to install world worldHost system into game system");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GAME_WORLD COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
