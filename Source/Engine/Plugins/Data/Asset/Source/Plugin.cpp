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
