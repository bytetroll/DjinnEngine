//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorldHost.h>
#include <Game/IWorldFeature.h>

DJINN_NS();

namespace {
}

DJINN_C_EXPORT_PLUGINS_UI_CONTROLS COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {

    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_UI_CONTROLS COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
