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
 on 10/22/2017.
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


