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
