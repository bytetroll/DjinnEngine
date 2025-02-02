//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/IStream.h>
#include <Data/IAssetCompiler.h>
#include "MaterialConfiguration.h"

namespace Djinn::Data {
    class MaterialCollectionAsset;
    class IMaterialLayout;
    class IMaterialType;

    class IMaterialCompiler : public IAssetCompiler {
    public:
        DJINN_COM_INTERFACE();

        IMaterialCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompiler(registry, outer, host) {

        }

        /// Register layout
        /// \param layout
        /// \return
        virtual COM::Result Register(IMaterialLayout* layout) = 0;

        /// Register type
        /// \param type
        /// \return
        virtual COM::Result Register(IMaterialType* type) = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAssetCompiler::kIID:
                    *out = static_cast<IAssetCompiler*>(this);
                    return COM::kOK;
                case IMaterialCompiler::kIID:
                    *out = static_cast<IMaterialCompiler*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}