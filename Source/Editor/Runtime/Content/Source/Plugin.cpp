//
// Created by Dev on 6/27/2018.
//

#include <Host/PluginAPI.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <UI/IUIMLHost.h>
#include <Game/IWorld.h>
#include <Data/IAssetHost.h>
#include <Editor/IEditorWorld.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_CONTENT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {

    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_CONTENT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
