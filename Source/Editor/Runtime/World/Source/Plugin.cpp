//
// Created by Dev on 6/27/2018.
//

#include <Host/PluginAPI.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Editor/EditorWorld.h>
#include <Base/DefaultLogs.h>
#include <Game/IGameSystem.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_WORLD COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        EditorWorld* world;
        if (!registry->CreateClass(plugin, &world) || !world->Initialize() || !registry->GetPipeline()->AddInterface(world)) {
            ErrorLog(registry).Write("Failed to create editor world");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_WORLD COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
