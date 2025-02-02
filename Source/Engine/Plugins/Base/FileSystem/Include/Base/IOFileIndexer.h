//
// Created by Dev on 3/20/2018.
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