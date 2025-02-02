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