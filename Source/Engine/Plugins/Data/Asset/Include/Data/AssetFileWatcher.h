//
// Created by Dev on 4/18/2018.
//
#pragma once

#include <Base/IFileWatcher.h>
#include <Core/String.h>
#include <Core/HashMap.h>
#include <Core/Delegate.h>

namespace Djinn::Data {
    struct AssetObject;
    class IAssetConfiguration;

    class DJINN_EXPORT_PLUGINS_DATA_ASSET AssetFileWatcher : public Base::IFileWatcher {
    public:
        DJINN_COM_CLASS();

        AssetFileWatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this watcher
        /// \return
        COM::Result Initialize();

        /// Register an asset
        /// \param object
        /// \param config
        /// \return
        COM::Result Register(const Base::PathID& path, const Core::Delegate<void(Base::FileEvent event)>& callback);

        /// Overrides
        void OnEvent(Base::FileEvent event, const Base::PathID &path) override;

    private:
        Core::HashMap<Base::PathID, Core::Delegate<void(Base::FileEvent event)>> entries;
    };
}