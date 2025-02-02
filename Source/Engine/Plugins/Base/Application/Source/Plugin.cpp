//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/Application.h>

DJINN_NS2(Base);

DJINN_C_EXPORT_PLUGINS_BASE_APPLICATION COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry) {
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_BASE_APPLICATION COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
