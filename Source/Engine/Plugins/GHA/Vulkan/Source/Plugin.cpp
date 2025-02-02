//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <GHA/Vulkan/Adapter.h>

DJINN_NS3(GHA, Vulkan);

DJINN_C_EXPORT_PLUGINS_GHA_VULKAN COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet) {
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GHA_VULKAN COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
