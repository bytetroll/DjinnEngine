//
// Created by Dev on 4/18/2018.
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
