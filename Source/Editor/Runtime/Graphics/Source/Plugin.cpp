//
// Created by Dev on 6/27/2018.
//

#include <Host/PluginAPI.h>

DJINN_NS();

DJINN_C_EXPORT_EDITOR_GRAPHICS COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_GRAPHICS COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
