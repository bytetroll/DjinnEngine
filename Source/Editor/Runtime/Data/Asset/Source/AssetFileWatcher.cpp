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
 on 4/18/2018.
//

#include <Editor/AssetFileWatcher.h>
#include <Editor/IAssetRegistry.h>
#include <Data/Asset.h>
#include <Base/IFileProxy.h>
#include <Base/IFileSystem.h>
#include <Base/DefaultLogs.h>
#include <Editor/IndexedDirectory.h>

DJINN_NS2(Editor);

AssetFileWatcher::AssetFileWatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileWatcher(registry, outer, host) {

}

COM::Result AssetFileWatcher::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&assetRegistry)) {
        ErrorLog(this).Write("Expected asset registry to be installed");
        return COM::kError;
    }

    if (!registry->GetPipeline()->GetInterface(&fs)) {
        ErrorLog(this).Write("Expected file system to be installed");
        return COM::kError;
    }

    return COM::kOK;
}

void AssetFileWatcher::OnEvent(Base::FileEvent event, const Base::PathID &path) {
    // Only care about registration events
    if (event != Base::FileEvent::eRegistered) {
        return;
    }

    // Get file proxy
    Base::IFileProxy *proxy;
    if (!fs->GetFileProxy(path, &proxy)) {
        return;
    }

    // Get extension, assets end with ".daf".
    Core::StringView view;
    if (!proxy->GetVirtualPath(path, &view) || !view.EndsWith(".daf")) {
        return;
    }

    // Register as asset
    Sink(assetRegistry->Register(path));

    // Register in directory
    GetDirectory(view)->Register(path);
}

IIndexedDirectory *AssetFileWatcher::GetDirectory(const Core::StringView &path) {
    if (auto begin = path.LastIndexOf("/"); begin != -1) {
        auto folder = path.SubStr(0, begin);

        auto &dir = directories[folder];
        if (!dir) {
            auto parent = GetDirectory(folder);

            // Get directory name
            auto name = folder;
            if (auto begin = name.LastIndexOf("/"); begin != -1) {
                name = folder.SubStr(begin + 1);
            }

            // Create directory
            if (!registry->CreateClass(this, &dir) || !dir->Initialize(name)) {
                ErrorLog(this).Write("Failed to create indexed directory");
                return nullptr;
            }

            parent->Register(dir);
        }

        return dir;
    }

    return assetRegistry->GetDirectory();
}
