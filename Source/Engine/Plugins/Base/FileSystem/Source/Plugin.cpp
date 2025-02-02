//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/16/2017.
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
