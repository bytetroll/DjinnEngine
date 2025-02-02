//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Editor/IEditorWorld.h>
#include <Game/IWorld.h>
#include <Data/IAssetHost.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_IMPORTERS_MESH COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        // Get editor world
        IEditorWorld *world;
        if (!registry->GetPipeline()->GetInterface(&world)) {
            ErrorLog(registry).Write("Expected editor world to be installed");
            return COM::kOK;
        }

    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_IMPORTERS_MESH COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    return COM::kOK;
}
