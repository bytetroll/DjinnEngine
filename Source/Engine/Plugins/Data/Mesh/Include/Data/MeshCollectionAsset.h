//
// Created by Dev on 10/25/2017.
//
#pragma once

#include <Data/IAsset.h>
#include <Core/Array.h>

namespace Djinn::Data {
    class IMeshAsset;

    class MeshCollectionAsset : public IAsset {
    public:
        DJINN_COM_CLASS();

        MeshCollectionAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides

        // Meshes
        Core::Array<IMeshAsset*> meshes;
    };
}