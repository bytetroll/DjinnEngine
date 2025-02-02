//
// Created by Dev on 4/18/2018.
//
#pragma once

#include <Base/IFileWatcher.h>
#include <Core/String.h>
#include <Core/HashMap.h>
#include <Core/Delegate.h>
#include <Core/StringView.h>

namespace Djinn::Base {
    class IFileSystem;
}

namespace Djinn::Editor {
    class IAssetRegistry;
    class IIndexedDirectory;
    class IndexedDirectory;

    class DJINN_EXPORT_EDITOR_DATA_ASSET AssetFileWatcher : public Base::IFileWatcher {
    public:
        DJINN_COM_CLASS();

        AssetFileWatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this watcher
        /// \return
        COM::Result Initialize();

        /// Overrides
        void OnEvent(Base::FileEvent event, const Base::PathID &path) override;

    private:
        IIndexedDirectory* GetDirectory(const Core::StringView& path);

    private:
        IAssetRegistry* assetRegistry;
        Base::IFileSystem* fs;
        Core::HashMap<Core::StringView, IndexedDirectory*> directories;
    };
}