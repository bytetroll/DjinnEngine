//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Data/AssetHost.h>
#include <Base/DefaultLogs.h>
#include <Data/AssetCompilerRegistry.h>
#include <Platform/FileSystem.h>
#include <Data/AssetFileWatcher.h>

DJINN_NS2(Data);

DJINN_C_EXPORT_PLUGINS_DATA_ASSET COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        AssetCompilerRegistry *assetCompilerRegistry;
        if (!registry->CreateClass(plugin, &assetCompilerRegistry) || !registry->GetPipeline()->AddInterface(assetCompilerRegistry)) {
            ErrorLog(registry).Write("Failed to install asset compiler registry");
            return COM::kError;
        }

        AssetHost *assetHost;
        if (!registry->CreateClass(plugin, &assetHost) || !assetHost->Initialize() || !registry->GetPipeline()->AddInterface(assetHost)) {
            ErrorLog(registry).Write("Failed to install asset worldHost");
            return COM::kError;
        }

        // Ensure AssetCache/ folder exists
        if (!Platform::FileSystem::Exists("AssetCache")) {
            Platform::FileSystem::CreateDirectory("AssetCache");
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_DATA_ASSET COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    Sink(registry->GetPipeline()->RemoveInterface(IAssetHost::kIID));
    Sink(registry->GetPipeline()->RemoveInterface(IAssetCompilerRegistry::kIID));
    return COM::kOK;
}
