//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Game/InputSystem.h>
#include <Game/WindowInputDevice.h>
#include <Base/DefaultLogs.h>
#include <Game/IGameSystem.h>
#include <Game/IEntitySystem.h>
#include <Base/WindowSystem.h>
#include <Game/IWorldHost.h>
#include <Game/InputWorldPackage.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

namespace {
    //Game::InputConnection escapeConnection;
}

DJINN_C_EXPORT_PLUGINS_GAME_INPUT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get game system
        Game::IGameSystem* gameSystem;
        if (!registry->GetPipeline()->GetInterface(&gameSystem)) {
            ErrorLog(registry).Write("Input plugin expected an installed game system");
            return COM::kError;
        }

        // Install system
        Game::InputSystem* system;
        if (!registry->CreateClass(plugin, &system) || !system->Initialize() || !registry->GetPipeline()->AddInterface(system) || !gameSystem->Register(system)) {
            ErrorLog(registry).Write("Failed to install input system into game system");
            return COM::kError;
        }

        // Has window system?
        Base::IWindowSystem* windowSystem;
        if (registry->GetPipeline()->GetInterface(&windowSystem)) {
            Game::WindowInputDevice* device;
            if (!registry->CreateClass(plugin, &device) || !device->Initialize(windowSystem->GetWindow()) || !system->Register(device)) {
                ErrorLog(registry).Write("Failed to install the window input device into the input system");
                return COM::kError;
            }

            // Reroute exit by default.
            Check(system->BindEvent(Game::InputEventType::eExit, [=](auto) {
                gameSystem->FlagExit();
            }, nullptr));
        }

        // Get world worldHost
        Game::IWorldHost *worldHost;
        if (!registry->GetPipeline()->GetInterface(&worldHost)) {
            ErrorLog(registry).Write("Input plugin expected an installed world worldHost");
            return COM::kError;
        }

        // Add package
        worldHost->GetFeature(Game::kDefaultFeature)->AddPackage<Game::InputWorldPackage>();
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GAME_INPUT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
