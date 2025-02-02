//
// Created by Dev on 10/22/2017.
//

#include <Base/IOFileIndexer.h>
#include <Base/FileSystem.h>
#include <Platform/FileSystem.h>
#include <Base/DefaultLogs.h>
#include <Base/IOFileProxy.h>
#include <Base/IFileWatcher.h>
#include <Async/Framework.h>
#include <Async/ScheduledJob.h>
#include <Platform/FileStream.h>
#include <Core/StringStream.h>

DJINN_NS2(Base);

IOFileIndexer::IOFileIndexer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileIndexer(registry, outer, host) {

}

COM::Result IOFileIndexer::Initialize(IFileSystem *fs, IOFileProxy *proxy) {
    this->fs = fs;
    this->proxy = proxy;

    // Get framework
    if (!registry->GetPipeline()->GetClass(&async)) {
        ErrorLog(this).Write("Expected async framework to be installed");
        return COM::kError;
    }

    // Mark as indexing
    fs->SetFlag(FileSystemFlag::eIndexing);

    // Get exe dir
    Core::String dir;
    if (!Platform::FileSystem::GetExecutableDirectory(dir)) {
        ErrorLog(this).Write("Failed to get executable directory");
        return COM::kError;
    }

    // Get executable directory
    Core::String directory;
    if (!Platform::FileSystem::GetExecutableDirectory(directory)) {
        ErrorLog(this).Write("Failed to open base directory for file watching");
        return COM::kError;
    }

    // Add base job
    IndexDirectory("", dir);

    // Start worker
    indexerWorker.SetName(Core::String(GetClassName()) + "Indexer");
    indexerWorker.Start(Bindff(IndexerWorker));

    // Start watcher worker
    auto &worker = watcherWorkers.Emplace();
    worker.SetName(Core::String(GetClassName()) + " Watcher");
    worker.Start(Bindff(WatcherWorker), Core::Pack(Core::String(""), directory));

    // Diagnostic
    InfoLog(this).Write("Initialized, indexer thread started");

    // OK
    return COM::kOK;
}

void IOFileIndexer::IndexerWorker() {
    for (;;) {
        // Wait for job
        jobVar.Wait(jobMtx, [&] { return pendingJobs.Size() > 0; });

        // Mark as indexing
        fs->SetFlag(FileSystemFlag::eIndexing);

        // Process jobs
        for (;;) {
            // Get job
            Job job;
            {
                Platform::LockGuard guard(jobMtx);
                job = pendingJobs[0];
                pendingJobs.Remove(0);
            }

            // Directory?
            if (job.isDirectory) {
                // InfoLog(this).Write("Indexing directory '", job.pattern, "'");

                // Glob files
                globCache.Clear(false);
                if (!Platform::FileSystem::Glob(globCache, job.pattern)) {
                    ErrorLog(this).Write("IO file indexer failed to index path: '", job.pattern, "'");
                }

                // Process files
                for (auto &path : globCache) {
                    if (Platform::FileSystem::IsDirectory(path)) {
                        Platform::LockGuard guard(jobMtx);
                        Job dirJob;
                        dirJob.isDirectory = true;
                        dirJob.pattern = Platform::FileSystem::CombinePaths(path, "*");
                        dirJob.drive = job.drive;
                        dirJob.basePath = job.basePath;
                        pendingJobs.Add(dirJob);
                    } else {
                        Core::String virtualPath = Core::String(path, job.basePath.Length(), path.Length());
                        if (job.drive != "") {
                            virtualPath = Platform::FileSystem::CombinePaths(job.drive, virtualPath);
                        }
                        virtualPath.Replace('\\', '/');
                        PathID id = PathID(Core::ComputeCRC32(virtualPath.AsCharArray()));

                        Sink(fs->Register(id, proxy));
                        Sink(proxy->Register(id, path, virtualPath));

                        // Must be able to iterate watchers
                        UInt32 watcherCount;
                        IFileWatcher **watchers;
                        if (fs->EnumerateWatchers(watcherCount, watchers)) {
                            for (USize watcherIndex = 0; watcherIndex < watcherCount; watcherIndex++) {
                                watchers[watcherIndex]->OnEvent(FileEvent::eRegistered, id);
                            }
                        }

                        // Virtual Registry?
                        Core::String end = virtualPath;
                        end.ToLowercase();
                        if (end.EndsWith(".dvrf")) {
                            Core::String content;
                            if (Platform::FileSystem::LoadTextFile(path, content)) {
                                Int64 offset = content.IndexOf(':');
                                if (offset != -1) {
                                    auto begin = Core::String(content, 0, offset);
                                    auto end = Core::String(content, offset + 1, content.Length());

                                    Platform::LockGuard guard(jobMtx);
                                    Job dirJob;
                                    dirJob.isDirectory = true;
                                    dirJob.pattern = Platform::FileSystem::CombinePaths(end, "*");
                                    dirJob.drive = begin;
                                    dirJob.basePath = end;
                                    pendingJobs.Add(dirJob);

                                    // Start watcher worker
                                    auto &worker = watcherWorkers.Emplace();
                                    worker.SetName(Core::String(GetClassName()) + " Watcher");
                                    worker.Start(Bindff(WatcherWorker), Core::Pack(begin, end));
                                }
                            }
                        }
                    }
                }
            } else {

            }

            // Any job available?
            {
                Platform::LockGuard guard(jobMtx);
                if (pendingJobs.Size() == 0) {
                    // Unmark as indexing
                    fs->RemoveFlag(FileSystemFlag::eIndexing);
                    break;
                }
            }
        }
    }
}

void IOFileIndexer::IndexDirectory(const Core::String &drive, const Core::String &pattern) {
    Job job;
    job.isDirectory = true;
    if (Platform::FileSystem::IsDirectory(pattern)) {
        job.pattern = Platform::FileSystem::CombinePaths(pattern, "*");
    } else {
        job.pattern = pattern;
    }
    job.drive = drive;
    job.basePath = Platform::FileSystem::GetDirectory(pattern);
    pendingJobs.Add(job);
}

void IOFileIndexer::WatcherWorker(Core::String drive, Core::String directory) {
    // Diagnostic
    InfoLog(this).Write("Started watcher for '", directory, "'");

    UInt32 count;
    Platform::FileEvent events[1024];
    while (Platform::FileSystem::WaitDirectoryEvent(directory, 1024, count, events)) {
        for (UInt32 i = 0; i < count; i++) {
            Core::String virtualPath = events[i].path;
            if (drive != "") {
                virtualPath = Platform::FileSystem::CombinePaths(drive, virtualPath);
            }
            virtualPath.Replace('\\', '/');
            PathID pathID = PathID(Core::ComputeCRC32(virtualPath.AsCharArray()));

            // Must be able to iterate watchers
            UInt32 watcherCount;
            IFileWatcher **watchers;
            if (fs->EnumerateWatchers(watcherCount, watchers)) {
                for (USize watcherIndex = 0; watcherIndex < watcherCount; watcherIndex++) {
                    switch (events[i].type) {
                        case Platform::FileEventType::eWrite:
                            watchers[watcherIndex]->OnEvent(FileEvent::eModified, pathID);
                            break;
                        case Platform::FileEventType::eModified:
                            watchers[watcherIndex]->OnEvent(FileEvent::eModified, pathID);
                            break;
                        case Platform::FileEventType::eCreated:
                            watchers[watcherIndex]->OnEvent(FileEvent::eCreated, pathID);
                            break;
                        case Platform::FileEventType::eDeleted:
                            watchers[watcherIndex]->OnEvent(FileEvent::eDeleted, pathID);
                            break;
                    }
                }
            }
        }
    }

    // Diagnostic
    WarningLog(this).Write("Stopped watcher for '", directory, "'");
}
