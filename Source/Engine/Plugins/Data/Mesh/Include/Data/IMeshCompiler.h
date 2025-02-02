//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/IStream.h>
#include <Data/IAssetCompiler.h>
#include "MeshConfiguration.h"

namespace Djinn::Data {
    class MeshCollectionAsset;

    class DJINN_EXPORT_PLUGINS_DATA_MESH IMeshCompiler : public IAssetCompiler {
    public:
        DJINN_COM_INTERFACE();

        IMeshCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompiler(registry, outer, host) {

        }

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAssetCompiler::kIID:
                    *out = static_cast<IAssetCompiler*>(this);
                    return COM::kOK;
                case IMeshCompiler::kIID:
                    *out = static_cast<IMeshCompiler*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}