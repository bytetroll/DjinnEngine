//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/ISystem.h>
#include <Base/DefaultLogs.h>
#include <Base/Application.h>
#include <Base/WindowSystem.h>
#include <Platform/User.h>

DJINN_NS2(Base);

DJINN_C_EXPORT_PLUGINS_BASE_WINDOW COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        Base::Application *app;
        if (!registry->GetPipeline()->GetClass(&app)) {
            ErrorLog(registry).Write("Failed to initialize window plugin, expected application to be installed");
            return COM::kError;
        }

        WindowSystem *system;
        if (!registry->CreateClass(plugin, &system) || !system->Initialize(
                Platform::WindowDesc()
                        .Width(Math::Min(1920u, Platform::User::GetPrimaryDisplayWidth()))
                        .Height(Math::Min(1080u, Platform::User::GetPrimaryDisplayHeight()))
                        .Title(app->GetInfo().name)
        ) || !app->Register(system) || !registry->GetPipeline()->AddInterface(system)) {
            ErrorLog(registry).Write("Failed to initialize window plugin, failed to install window system");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_BASE_WINDOW COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
