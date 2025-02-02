//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>

DJINN_NS();

DJINN_C_EXPORT_EDITOR_DATA_LEVEL COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_DATA_LEVEL COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    return COM::kOK;
}
