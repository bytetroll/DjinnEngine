//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Configuration/ConfigFileManager.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Configuration);

DJINN_C_EXPORT_PLUGINS_CONFIGURATION COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flag, Host::IPlugin* plugin) {
    if (flag & Host::LoadFlag::ePipeline) {
        // Create manager
        ConfigFileManager* manager;
        if (!registry->CreateClass(plugin, &manager) || !registry->GetPipeline()->AddClass(manager)) {
            ErrorLog(registry).Write("Failed to load configuration plugin, failed to create config file manager");
            return COM::kError;
        }
    }

    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_CONFIGURATION COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
