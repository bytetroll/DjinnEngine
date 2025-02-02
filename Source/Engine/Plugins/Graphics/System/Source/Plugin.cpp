//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/GraphicsSystem.h>
#include <GHA/IConfiguration.h>
#include <Base/DefaultLogs.h>
#include <Game/IGameSystem.h>
#include <Base/IWindowSystem.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_SYSTEM COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get game system
        Game::IGameSystem *gameSystem;
        if (!registry->GetPipeline()->GetInterface(&gameSystem)) {
            ErrorLog(registry).Write("Input plugin expected an installed game system");
            return COM::kError;
        }

        // Get window system
        Base::IWindowSystem *windowSystem;
        if (!registry->GetPipeline()->GetInterface(&windowSystem)) {
            ErrorLog(registry).Write("Input plugin expected an installed window system");
            return COM::kError;
        }

        USize configCount;
        if (!registry->GetClasses(GHA::IConfiguration::kIID, configCount, nullptr)) {
            ErrorLog(registry).Write("Failed to get configuration classes");
            return COM::kOK;
        }

        Core::Array<COM::ClassID> configs(configCount);
        if (!registry->GetClasses(GHA::IConfiguration::kIID, configCount, configs.Ptr())) {
            ErrorLog(registry).Write("Failed to get configuration classes");
            return COM::kOK;
        }

        if (configCount == 0) {
            ErrorLog(registry).Write("Graphics system plugin expects one or more gha configurations to be registry installed");
            return COM::kOK;
        }

        GHA::IConfiguration *config;
        if (!registry->CreateClass(configs[0], plugin, &config)) {
            ErrorLog(registry).Write("Failed to create gha config with class id ", configs[0].Get());
            return COM::kOK;
        }

        Graphics::GraphicsSystem *system;
        if (!registry->CreateClass(plugin, &system)
            || !system->Initialize()
            || !system->Configure(config, windowSystem->GetWindow())
            || !registry->GetPipeline()->AddInterface(system)
            || !gameSystem->Register(system)) {
            ErrorLog(registry).Write("Failed to install graphics system");
            return COM::kOK;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_SYSTEM COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
