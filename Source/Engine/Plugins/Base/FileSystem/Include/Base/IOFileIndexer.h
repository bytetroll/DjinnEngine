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
 on 3/20/2018.
//

#include <Base/IFileIndexer.h>
#include <Platform/Thread.h>
#include <Core/SlotArray.h>
#include <Platform/Event.h>
#include <Core/Array.h>
#include <Core/List.h>

namespace Djinn::Async {
    class Framework;
}

namespace Djinn::Base {
    class IFileSystem;
    class IOFileProxy;

    class DJINN_EXPORT_PLUGINS_BASE_FILESYSTEM IOFileIndexer : public IFileIndexer {
    public:
        DJINN_COM_CLASS();

        IOFileIndexer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this indexer
        /// \return
        COM::Result Initialize(IFileSystem* fs, IOFileProxy* proxy);

        /// Index directory into given drive
        /// \param drive - Fx. "Shaders"
        /// \param pattern - Fx. "Runtime/Shaders/*.dsf"
        void IndexDirectory(const Core::String& drive, const Core::String& pattern);

    private:
        void IndexerWorker();
        void WatcherWorker(Core::String drive, Core::String directory);

    private:
        Platform::Thread indexerWorker;
        Core::List<Platform::Thread> watcherWorkers;
        IOFileProxy* proxy;
        IFileSystem* fs;
        Async::Framework* async;

        struct Job {
            bool isDirectory;
            Core::String pattern;
            Core::String basePath;
            Core::String drive;
        };
        Platform::Mutex jobMtx;
        Platform::ConditionalVar jobVar;
        Core::SlotArray<Job> pendingJobs;

        Core::Array<Core::String> globCache;
    };
}