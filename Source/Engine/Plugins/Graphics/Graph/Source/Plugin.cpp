//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/Graph.h>
#include <Graphics/Pipeline.h>
#include <Graphics/RootProcess.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_GRAPH COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet) {
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_GRAPH COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}

