//
// Created by Dev on 10/22/2017.
//

#include <Base/FileSystem.h>
#include <Core/Common.h>
#include <Platform/FileSystem.h>
#include <Base/IOFileProxy.h>
#include <Host/IRegistry.h>
#include <Base/FileHandle.h>
#include <Platform/LockGuard.h>
#include <Base/DefaultLogs.h>
#include <Base/IFileWatcher.h>

DJINN_NS2(Base);

FileSystem::FileSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileSystem(registry, outer, host) {

}

COM::Result FileSystem::Register(const PathID &path, IFileProxy *proxy) {
    Platform::LockGuard guard(mutex);
    proxyMap.Add(path, proxy);

    // Invoke watchers
    for (auto& watcher : watchers) {
        watcher->OnEvent(Base::FileEvent::eRegistered, path);
    }
    return COM::kOK;
}

COM::Result FileSystem::Deregister(const PathID &path) {
    Platform::LockGuard guard(mutex);
    return proxyMap.Remove(path) ? COM::kOK : COM::kError;
}

COM::Result FileSystem::OpenStream(const PathID &path, Platform::FileMode mode, FileHandle *out) {
    // Must not be indexing
    flagVar.Wait(flagMtx, [&] { return !(flags & FileSystemFlag::eIndexing); });

    Platform::LockGuard guard(mutex);

    // Find proxy
    auto it = proxyMap.Find(path);
    if (it == proxyMap.End()) {
        return COM::kError;
    }

    auto proxy = (*it).second;

    // Attempt to open stream
    Core::IStream *stream;
    if (!proxy->OpenStream(path, mode, &stream)) {
        return COM::kError;
    }

    // Create handle
    out->Open(path, stream, proxy);
    return COM::kOK;
}

COM::Result FileSystem::Register(IFileWatcher *watcher, bool submitEvents) {
    Platform::LockGuard guard(mutex);
    watchers.Add(watcher);

    if (submitEvents) {
        for (auto&& kv : proxyMap) {
            watcher->OnEvent(FileEvent::eRegistered, kv.first);
        }
    }
    return COM::kOK;
}

COM::Result FileSystem::Deregister(IFileWatcher *watcher) {
    Platform::LockGuard guard(mutex);
    return watchers.RemoveValue(watcher) ? COM::kOK : COM::kError;
}

COM::Result FileSystem::EnumerateWatchers(UInt32 &count, IFileWatcher **&_watchers) {
    Platform::LockGuard guard(mutex);
    count = static_cast<UInt32>(watchers.Size());
    _watchers = watchers.Ptr();
    return COM::kOK;
}

COM::Result FileSystem::Register(IFileIndexer *indexer) {
    Platform::LockGuard guard(mutex);
    indexers.Add(indexer);
    return COM::kOK;
}

COM::Result FileSystem::Deregister(IFileIndexer *indexer) {
    Platform::LockGuard guard(mutex);
    return indexers.RemoveValue(indexer) ? COM::kOK : COM::kError;
}

void FileSystem::SetFlag(FileSystemFlag flags) {
    {
        Platform::LockGuard guard(flagMtx);
        this->flags += flags;
    }
    flagVar.SignalAll();
}

void FileSystem::RemoveFlag(FileSystemFlag flags) {
    {
        Platform::LockGuard guard(flagMtx);
        this->flags -= flags;
    }
    flagVar.SignalAll();
}

COM::Result FileSystem::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result FileSystem::GetFileProxy(const PathID &path, IFileProxy **out) {
    // Must not be indexing
    flagVar.Wait(flagMtx, [&] { return !(flags & FileSystemFlag::eIndexing); });

    Platform::LockGuard guard(mutex);

    // Find proxy
    auto it = proxyMap.Find(path);
    if (it == proxyMap.End()) {
        return COM::kError;
    }

    // ...
    *out = (*it).second;
    return COM::kOK;
}


