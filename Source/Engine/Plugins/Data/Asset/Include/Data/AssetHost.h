//
// Created by Dev on 10/18/2017.
//
#pragma once

#include "IAssetHost.h"
#include <Serialization/ISerializerHost.h>
#include <Core/HashMap.h>

namespace Djinn::Base {
    class IFileSystem;
}

namespace Djinn::Data {
    class IAssetCompilerRegistry;
    class AssetFileWatcher;

    class DJINN_EXPORT_PLUGINS_DATA_ASSET AssetHost : public IAssetHost {
    public:
        DJINN_COM_CLASS();

        AssetHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this worldHost
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result CompileAsset(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;
        virtual COM::Result CompileAsset(const Base::PathID &path, IAssetConfiguration *config, IAsset **out) override;
        virtual COM::Result CompileAsset(const Base::PathID &path, IAsset **out) override;
        virtual void Register(const Core::String &extension, IFileAssetCompiler *compiler) override;
        virtual COM::Result LoadAsset(Core::IStream *stream, IAsset **out) override;
        virtual COM::Result StoreAssetStream(Core::IStream *stream, IAsset *asset) override;
        virtual COM::Result LoadAsset(const Base::PathID &path, IAsset **out) override;
        virtual COM::Result StoreAsset(const Core::String &path, IAsset *asset) override;
        virtual AssetObject *CompileAssetObjectAsync(Core::IStream *stream, IAssetConfiguration *config) override;
        virtual AssetObject *CompileAssetObjectAsync(const Base::PathID &path, IAssetConfiguration *config) override;
        virtual AssetObject *CompileAssetObjectAsync(const Base::PathID &path) override;
        virtual AssetObject *LoadAssetObjectAsync(Core::IStream *stream) override;
        virtual AssetObject *LoadAssetObjectAsync(const Base::PathID &path) override;

    private:
        Base::IFileSystem *fileSystem = nullptr;
        AssetFileWatcher* fileWatcher = nullptr;
        IAssetCompilerRegistry *compilerRegistry = nullptr;
        Serialization::ISerializerHost *serializerHost = nullptr;
        Core::HashMap<Core::String, IFileAssetCompiler *> fileCompilers;
        Core::HashMap<Base::PathID, AssetObject*> assetObjects;

    private:
        Platform::SpinMutex mutex;
        Core::ObjectPool<AssetObject> asyncObjects;
    };
}