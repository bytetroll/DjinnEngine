//
// Created by Dev on 10/22/2017.
//
#pragma once

#include <Core/HashMap.h>
#include <Core/Array.h>
#include <Platform/ConditionalVar.h>
#include <Core/String.h>
#include <Platform/RecursiveSpinMutex.h>
#include "IFileSystem.h"

namespace Djinn::Base {
    /// Thread Safe
    class DJINN_EXPORT_PLUGINS_BASE_FILESYSTEM FileSystem : public IFileSystem {
    public:
        DJINN_COM_CLASS();

        FileSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this file system
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Register(const PathID &path, IFileProxy *proxy) override;
        virtual COM::Result Deregister(const PathID &path) override;
        virtual COM::Result OpenStream(const PathID &path, Platform::FileMode mode, FileHandle* out) override;
        virtual COM::Result Register(IFileWatcher *watcher, bool submitEvents) override;
        virtual COM::Result Deregister(IFileWatcher *watcher) override;
        virtual COM::Result Register(IFileIndexer *indexer) override;
        virtual COM::Result Deregister(IFileIndexer *indexer) override;
        virtual COM::Result EnumerateWatchers(UInt32 &count, IFileWatcher **&watchers) override;
        virtual COM::Result GetFileProxy(const PathID &path, IFileProxy **out) override;
        virtual void SetFlag(FileSystemFlag flags) override;
        virtual void RemoveFlag(FileSystemFlag flags) override;

    private:
        Platform::RecursiveSpinMutex mutex;
        Platform::Mutex flagMtx;
        FileSystemFlagSet flags;
        Platform::ConditionalVar flagVar;
        Core::HashMap<PathID, IFileProxy*> proxyMap;
        Core::Array<IFileWatcher*> watchers;
        Core::Array<IFileIndexer*> indexers;
    };
}