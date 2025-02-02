//
// Created by Dev on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Base/FileSystem.h>
#include <Base/IOFileProxy.h>
#include <Base/DefaultLogs.h>
#include <Base/IOFileIndexer.h>

DJINN_NS2(Base);

DJINN_C_EXPORT_PLUGINS_BASE_FILESYSTEM COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        // Create file system
        FileSystem *fileSystem;
        if (!registry->CreateClass(plugin, &fileSystem) || !fileSystem->Initialize() || !registry->GetPipeline()->AddInterface(fileSystem)) {
            ErrorLog(registry).Write("Failed to install file system");
            return COM::kError;
        }

        // Create standard file proxy
        IOFileProxy *ioFileProxy;
        if (!registry->CreateClass(plugin, &ioFileProxy) || !registry->GetPipeline()->AddInterface(ioFileProxy)) {
            ErrorLog(registry).Write("Failed to install io file proxy");
            return COM::kError;
        }

        // Create standard file indexer
        IOFileIndexer *ioFileIndexer;
        if (!registry->CreateClass(plugin, &ioFileIndexer) || !ioFileIndexer->Initialize(fileSystem, ioFileProxy) || !registry->GetPipeline()->AddInterface(ioFileIndexer) || !fileSystem->Register(ioFileIndexer)) {
            ErrorLog(registry).Write("Failed to install io file indexer");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_BASE_FILESYSTEM COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    // Clean up pipeline
    Check(registry->GetPipeline()->RemoveInterface(IFileSystem::kIID));
    Check(registry->GetPipeline()->RemoveInterface(IOFileProxy::kIID));
    return COM::kOK;
}
