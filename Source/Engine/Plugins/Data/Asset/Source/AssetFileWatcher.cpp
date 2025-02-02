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

#include <Data/AssetFileWatcher.h>
#include <Data/Asset.h>
#include <Data/IAssetConfiguration.h>
#include <Base/IFileSystem.h>
#include <Base/DefaultLogs.h>
#include <Async/Async.h>

DJINN_NS2(Data);

AssetFileWatcher::AssetFileWatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileWatcher(registry, outer, host) {

}

COM::Result AssetFileWatcher::Initialize() {
    Base::IFileSystem *fs;
    if (!registry->GetPipeline()->GetInterface(&fs)) {
        ErrorLog(this).Write("Expected file system to be installed");
        return COM::kError;
    }

    return COM::kOK;
}

COM::Result AssetFileWatcher::Register(const Base::PathID &path, const Core::Delegate<void(Base::FileEvent event)>& callback) {
    entries[path] = callback;
    return COM::kOK;
}

void AssetFileWatcher::OnEvent(Base::FileEvent event, const Base::PathID &path) {
    auto it = entries.Find(path);
    if (it == entries.End()) {
        return;
    }

    it->Invoke(event);
}
