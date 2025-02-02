//
// Created by Dev on 3/22/2018.
//
#pragma once

#include <Core/Array.h>
#include "IAssetRegistry.h"

namespace Djinn::Editor {
    class IndexedDirectory;

    class DJINN_EXPORT_EDITOR_DATA_ASSET AssetRegistry : public IAssetRegistry {
    public:
        DJINN_COM_CLASS();

        AssetRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this registry
        /// \return
        COM::Result Initialize();

        /// Overrides
        IIndexedDirectory *GetDirectory() override;
        COM::Result Register(const Base::PathID &id) override;

    private:
        Core::Array<Base::PathID> assets;

        /// Virtual root directory
        IndexedDirectory* directory;
    };
}