//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/DefaultLogs.h>
#include <Editor/AssetRegistry.h>
#include <Editor/AssetFileWatcher.h>
#include <Base/IFileSystem.h>

DJINN_NS2(Editor);

DJINN_C_EXPORT_EDITOR_DATA_ASSET COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        Base::IFileSystem* fs;
        if (!registry->GetPipeline()->GetInterface(&fs)) {
            ErrorLog(registry).Write("Expected file system to be installed");
            return COM::kError;
        }

        AssetRegistry *assetRegistry;
        if (!registry->CreateClass(plugin, &assetRegistry) || !assetRegistry->Initialize() || !registry->GetPipeline()->AddInterface(assetRegistry)) {
            ErrorLog(registry).Write("Failed to install asset registry");
            return COM::kError;
        }

        // Create asset watcher
        AssetFileWatcher *assetFileWatcher;
        if (!registry->CreateClass(plugin, &assetFileWatcher) || !assetFileWatcher->Initialize() || !fs->Register(assetFileWatcher, true)) {
            ErrorLog(registry).Write("Failed to create asset file watcher");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_EDITOR_DATA_ASSET COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    return COM::kOK;
}
