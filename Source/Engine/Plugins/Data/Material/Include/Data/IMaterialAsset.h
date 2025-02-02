//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Data/IAsset.h>
#include "MaterialQuery.h"

namespace Djinn::Data {
    class ShaderAsset;

    class IMaterialAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        IMaterialAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

        }

        /// Get shader
        /// \param query
        /// \param out
        /// \return
        virtual COM::Result GetShader(const MaterialQuery& query, ShaderAsset** out) = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAsset::kIID:
                    *out = static_cast<IAsset*>(this);
                    return COM::kOK;
                case IMaterialAsset::kIID:
                    *out = static_cast<IMaterialAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}