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