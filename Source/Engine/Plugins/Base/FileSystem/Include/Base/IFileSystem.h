//
// Created by Dev on 10/22/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Platform/FileMode.h>
#include "Paths.h"
#include "FileSystemFlag.h"

namespace Djinn::Core {
    class IStream;
}

namespace Djinn::Base {
    class IFileProxy;
    class IFileWatcher;
    class IFileIndexer;
    class FileHandle;

    /// Common results
    DJINN_COM_RESULT(UnknownDrive, true, "Base");
    DJINN_COM_RESULT(UnknownFile, true, "Base");
    DJINN_COM_RESULT(UnknownDirectory, true, "Base");

    class IFileSystem : public COM::TUnknown<IFileSystem> {
    public:
        DJINN_COM_INTERFACE();

        IFileSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a path to given proxy
        /// \param path
        /// \param proxy
        /// \return
        virtual COM::Result Register(const PathID &path, IFileProxy *proxy) = 0;

        /// Deregister a path
        /// \param path
        /// \return
        virtual COM::Result Deregister(const PathID &path) = 0;

        /// Register a file watcher
        /// \param watcher
        /// \return
        virtual COM::Result Register(IFileWatcher *watcher, bool submitEvents = false) = 0;

        /// Deregister a file indexer
        /// \param watcher
        /// \return
        virtual COM::Result Deregister(IFileWatcher *watcher) = 0;

        /// Register a file indexer
        /// \param watcher
        /// \return
        virtual COM::Result Register(IFileIndexer *indexer) = 0;

        /// Deregister a file watcher
        /// \param watcher
        /// \return
        virtual COM::Result Deregister(IFileIndexer *indexer) = 0;

        /// Enumerate all watchers
        /// \param count
        /// \param watchers
        /// \return
        virtual COM::Result EnumerateWatchers(UInt32 &count, IFileWatcher **&watchers) = 0;

        /// Open a stream to given path
        /// \param path
        /// \param mode - The file proxy may reject the mode if incompatible
        /// \param out
        /// \return
        virtual COM::Result OpenStream(const PathID &path, Platform::FileMode mode, FileHandle *out) = 0;

        /// Get file proxy of supplied path
        /// \param path
        /// \param out
        /// \return
        virtual COM::Result GetFileProxy(const PathID &path, IFileProxy** out) = 0;

        /// Enable a flag
        /// \param flags
        virtual void SetFlag(FileSystemFlag flags) = 0;

        /// Disable a flag
        /// \param flags
        virtual void RemoveFlag(FileSystemFlag flags) = 0;
    };
}