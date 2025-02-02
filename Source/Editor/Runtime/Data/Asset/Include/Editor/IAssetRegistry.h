//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Base/Paths.h>

namespace Djinn::Editor {
    class IAsset;
    class IIndexedDirectory;

    class IAssetRegistry : public COM::TUnknown<IAssetRegistry> {
    public:
        DJINN_COM_INTERFACE();

        IAssetRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a path to an asset
        /// \param id
        /// \return
        virtual COM::Result Register(const Base::PathID& id) = 0;

        /// Get the base directory
        /// \return
        virtual IIndexedDirectory* GetDirectory() = 0;
    };
}