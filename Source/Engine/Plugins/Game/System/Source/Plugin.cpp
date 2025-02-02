//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Game/GameSystem.h>
#include <Base/Application.h>
#include <Base/DefaultLogs.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GAME_SYSTEM COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get application
        Base::Application *app;
        if (!registry->GetPipeline()->GetClass(&app)) {
            ErrorLog(registry).Write("GameSystem plugin expected an installed Application");
            return COM::kError;
        }

        // Install system
        Game::GameSystem *system;
        if (!registry->CreateClass(plugin, &system) || !system->Initialize() || !registry->GetPipeline()->AddInterface(system) || !app->Register(system)) {
            ErrorLog(registry).Write("Failed to install game system into application");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GAME_SYSTEM COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
