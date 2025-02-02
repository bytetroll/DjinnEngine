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
 on 10/18/2017.
//

#include <Data/AssetHost.h>
#include <Core/Common.h>
#include <Data/IAssetSerializer.h>
#include <Data/AssetFileWatcher.h>
#include <Data/IAsset.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Data/IAssetCompilerRegistry.h>
#include <Data/IAssetCompiler.h>
#include <Data/IAssetConfiguration.h>
#include <Platform/FileSystem.h>
#include <Platform/FileStream.h>
#include <Data/IFileAssetCompiler.h>
#include <Base/IFileProxy.h>
#include <Base/FileSystem.h>
#include <Base/FileHandle.h>
#include <Async/Async.h>

DJINN_NS2(Data);

AssetHost::AssetHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetHost(registry, outer, host) {

}


COM::Result AssetHost::LoadAsset(Core::IStream *stream, IAsset **out) {
    Serialization::Archive archive(serializerHost, stream);

    COM::IUnknown *obj;
    if (COM::CheckedResult result = serializerHost->Deserialize(archive, &obj)) {
        ErrorLog(this).Write("Asset loading failed, failed to deserialize");
        return result;
    }

    if (COM::CheckedResult result = obj->QueryInterface(out)) {
        ErrorLog(this).Write("Failed to load asset, deserialized object not of IAsset interface type");
        return result;
    }

    return COM::kOK;
}

COM::Result AssetHost::StoreAssetStream(Core::IStream *stream, IAsset *asset) {
    Serialization::Archive archive(serializerHost, stream);
    return serializerHost->Serialize(archive, asset);
}

COM::Result AssetHost::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&serializerHost)) {
        ErrorLog(this).Write("Serialization worldHost not registered");
        return COM::kError;
    }

    if (!registry->GetPipeline()->GetInterface(&compilerRegistry)) {
        ErrorLog(this).Write("Asset compiler registry not registered");
        return COM::kError;
    }

    if (!registry->GetPipeline()->GetInterface(&fileSystem)) {
        ErrorLog(this).Write("File system not registered");
        return COM::kError;
    }

    if (!registry->CreateClass(this, &fileWatcher) || !fileWatcher->Initialize() || !fileSystem->Register(fileWatcher)) {
        ErrorLog(this).Write("Failed to create asset file watcher");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    return COM::kOK;
}

COM::Result AssetHost::CompileAsset(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) {
    IAssetCompiler *compiler;
    if (!compilerRegistry->GetCompiler(config->GetClassID(), &compiler)) {
        ErrorLog(this).Write("Failed to compile asset, no compiler registered that handles configuration class: ", config->GetClassID().Get());
        return COM::kError;
    }

    // Attempt to compile
    return compiler->Compile(stream, config, out);
}

COM::Result AssetHost::CompileAsset(const Base::PathID &path, IAssetConfiguration *config, IAsset **out) {
    Base::IFileProxy *proxy;
    if (!fileSystem->GetFileProxy(path, &proxy)) {
        ErrorLog(this).Write("Failed to compile asset, file not found '", path.GetString(), "'");
        return COM::kError;
    }

    // Hashes
    USize configHash = config->Hash();
    USize pathHash = Hash(path);

    // Read stamps
    Int64 stamp;
    if (!proxy->GetStamp(path, &stamp)) {
        stamp = 0;
    }

    Core::String hashedPath = Core::String(pathHash);
    if (path.GetString()) {
        hashedPath += "-" + Core::String(path.GetString());
    }

    // Precompiled path
    Core::String precompiledPath = Platform::FileSystem::CombinePaths("AssetCache", Platform::FileSystem::SanitizeFilename(hashedPath, '-') + ".daf");

    // Existing asset?
    if (auto precompiledStream = Platform::FileStream(Platform::FileMode::eRead, precompiledPath)) {
        // Read header
        Int64 serializedStamp;
        precompiledStream.Read(&serializedStamp, sizeof(Int64));
        USize serializedConfigHash;
        precompiledStream.Read(&serializedConfigHash, sizeof(USize));

        // If match, load at current position
        if (serializedStamp == stamp && serializedConfigHash == configHash) {
            return LoadAsset(&precompiledStream, out);
        }

        // Close
        precompiledStream.Close();
    }

    // Attempt to open file stream
    Base::FileHandle handle;
    if (!fileSystem->OpenStream(path, Platform::FileMode::eRead, &handle)) {
        ErrorLog(this).Write("Failed to compile asset, failed to open stream '", path.GetString(), "'");
        return COM::kError;
    }

    // Attempt to compile
    if (!CompileAsset(handle, config, out)) {
        return COM::kError;
    }

    // Open cache file
    Platform::FileStream precompiledStream(Platform::FileMode::eWrite, precompiledPath);
    if (!precompiledStream.IsGood()) {
        WarningLog(this).Write("Asset utility failed to open asset cache file handle, will not be cached");
        return COM::kOK;
    }

    // Write header
    precompiledStream.Write(&stamp, sizeof(Int64));
    precompiledStream.Write(&configHash, sizeof(USize));

    // Serialize
    if (!StoreAssetStream(&precompiledStream, *out)) {
        WarningLog(this).Write("Failed to cache compiled asset");
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

COM::Result AssetHost::CompileAsset(const Base::PathID &path, IAsset **out) {
    Base::IFileProxy *proxy = nullptr;
    if (!fileSystem->GetFileProxy(path, &proxy)) {
        ErrorLog(this).Write("Failed to compile asset, file not found '", path.GetString(), "'");
        return COM::kError;
    }

    // Auto deduction of asset configuration requires an absolute path
    Core::StringView absolute;
    if (!proxy->GetAbsolutePath(path, &absolute)) {
        ErrorLog(this).Write("Auto deduction of asset configuration for compilation requires an available absolute path '", path.GetString(), "'");
        return COM::kError;
    }

    // ...
    Int32 end = absolute.LastIndexOf('.');
    if (end == -1) {
        ErrorLog(this).Write("Failed to compile asset from path, no registered file compiler with file pattern '", absolute, "'");
        return kUnknownAssetExtension;
    }

    // Get extension
    auto ext = Core::String(absolute.Ptr(), end, absolute.Length());

    // Attempt to find compiler
    auto it = fileCompilers.Find(ext);
    if (it == fileCompilers.End()) {
        ErrorLog(this).Write("Failed to compile asset from path, no registered file compiler with file pattern '", absolute, "'");
        return kUnknownAssetExtension;
    }

    // Compile
    return (*it).second->Compile(path, absolute.Ptr(), ext, out);
}

void AssetHost::Register(const Core::String &extension, IFileAssetCompiler *compiler) {
    fileCompilers.Add(extension, compiler);
}

COM::Result AssetHost::LoadAsset(const Base::PathID &path, IAsset **out) {
    Base::FileHandle handle;
    if (!fileSystem->OpenStream(path, Platform::FileMode::eRead, &handle)) {
        ErrorLog(this).Write("Failed to load asset, file not found '", path.GetString(), "'");
        return COM::kError;
    }

    // Load from stream
    return LoadAsset(handle, out);
}

COM::Result AssetHost::StoreAsset(const Core::String &path, IAsset *asset) {
    Platform::FileStream stream(Platform::FileMode::eWrite, path);
    if (!stream.IsGood()) {
        ErrorLog(this).Write("Failed to store asset, file path '", path, "' could not be opened for write");
        return COM::kError;
    }

    // Store to stream
    return StoreAssetStream(&stream, asset);
}

AssetObject *AssetHost::CompileAssetObjectAsync(Core::IStream *stream, IAssetConfiguration *_config) {
    Platform::LockGuard guard(mutex);

    // Copy
    auto config = _config->Copy();

    // Create object
    auto object = asyncObjects.PopConstruct();

    // Async task
    object->task = Async::ScheduleTask<IAsset*>([=] {
        IAsset *asset;
        if (!CompileAsset(stream, config, &asset)) {
            asset = nullptr;
        }
        return asset;
    }, Async::kResource);

    // OK
    return object;
}

AssetObject *AssetHost::CompileAssetObjectAsync(const Base::PathID &path, IAssetConfiguration *_config) {
    Platform::LockGuard guard(mutex);

    // Already compiled / compiling?
    if (auto obj = assetObjects[path]) {
        return obj;
    }

    // Copy
    auto config = _config->Copy();

    // Create object
    auto object = asyncObjects.PopConstruct();
    assetObjects.Add(path, object);

    // Async task
    object->task = Async::ScheduleTask<IAsset*>([=] {
        IAsset *asset;
        if (!CompileAsset(path, config, &asset)) {
            asset = nullptr;
        }
        return asset;
    }, Async::kResource);

    // Register
    Sink(fileWatcher->Register(path, [=](Base::FileEvent event) {
        if (event == Base::FileEvent::eModified || event == Base::FileEvent::eCreated) {
            Core::StringView absolute;

            // Attempt to get absolute path
            Base::IFileProxy *proxy;
            if (!fileSystem->GetFileProxy(path, &proxy) || !proxy->GetAbsolutePath(path, &absolute)) {
                absolute = path.GetString();
            }

            // Diagnostic
            InfoLog(this).Write("Hot reloading asset: ", absolute);

            // Async task
            object->task = Async::ScheduleTask<IAsset *>([=] {
                IAsset *asset;
                if (!CompileAsset(path, config, &asset)) {
                    asset = nullptr;
                }
                return asset;
            }, Async::kResource);
        }
    }));

    // OK
    return object;
}

AssetObject *AssetHost::CompileAssetObjectAsync(const Base::PathID &path) {
    Platform::LockGuard guard(mutex);

    // Already compiled / compiling?
    if (auto obj = assetObjects[path]) {
        return obj;
    }

    // Create object
    auto object = asyncObjects.PopConstruct();
    assetObjects.Add(path, object);

    // Async task
    object->task = Async::ScheduleTask<IAsset*>([=] {
        IAsset *asset;
        if (!CompileAsset(path, &asset)) {
            asset = nullptr;
        }
        return asset;
    }, Async::kResource);

    // Register
    Sink(fileWatcher->Register(path, [=](Base::FileEvent event) {
        if (event == Base::FileEvent::eModified || event == Base::FileEvent::eCreated) {
            Core::StringView absolute;

            // Attempt to get absolute path
            Base::IFileProxy *proxy;
            if (!fileSystem->GetFileProxy(path, &proxy) || !proxy->GetAbsolutePath(path, &absolute)) {
                absolute = path.GetString();
            }

            // Diagnostic
            InfoLog(this).Write("Hot reloading asset: ", absolute);

            // Async task
            object->task = Async::ScheduleTask<IAsset *>([=] {
                IAsset *asset;
                if (!CompileAsset(path, &asset)) {
                    asset = nullptr;
                }
                return asset;
            }, Async::kResource);
        }
    }));

    // OK
    return object;
}

AssetObject *AssetHost::LoadAssetObjectAsync(Core::IStream *stream) {
    Platform::LockGuard guard(mutex);

    // Create object
    auto object = asyncObjects.PopConstruct();

    // Async task
    object->task = Async::ScheduleTask<IAsset*>([=] {
        IAsset *asset;
        if (!LoadAsset(stream, &asset)) {
            asset = nullptr;
        }
        return asset;
    });

    // OK
    return object;
}

AssetObject *AssetHost::LoadAssetObjectAsync(const Base::PathID &path) {
    Platform::LockGuard guard(mutex);

    // Create object
    auto object = asyncObjects.PopConstruct();

    // Async task
    object->task = Async::ScheduleTask<IAsset*>([=] {
        IAsset *asset;
        if (!LoadAsset(path, &asset)) {
            asset = nullptr;
        }
        return asset;
    }, Async::kResource);

    // Register
    Sink(fileWatcher->Register(path, [=](Base::FileEvent event) {
        if (event == Base::FileEvent::eModified || event == Base::FileEvent::eCreated) {
            Core::StringView absolute;

            // Attempt to get absolute path
            Base::IFileProxy *proxy;
            if (!fileSystem->GetFileProxy(path, &proxy) || !proxy->GetAbsolutePath(path, &absolute)) {
                absolute = path.GetString();
            }

            // Diagnostic
            InfoLog(this).Write("Hot reloading asset: ", absolute);

            // Async task
            object->task = Async::ScheduleTask<IAsset *>([=] {
                IAsset *asset;
                if (!LoadAsset(path, &asset)) {
                    asset = nullptr;
                }
                return asset;
            }, Async::kResource);
        }
    }));

    // OK
    return object;
}