//
// Created by Dev on 7/16/2018.
//
#pragma once

#include <Editor/IAsset.h>

namespace Djinn::Data {
    class IMeshAsset;
}

namespace Djinn::Editor {
    class IMeshAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        IMeshAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

        }

        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case Data::IAsset::kIID:
                    *out = static_cast<Data::IAsset*>(this);
                    return COM::kOK;
                case IAsset::kIID:
                    *out = static_cast<IAsset*>(this);
                    return COM::kOK;
                case IMeshAsset::kIID:
                    *out = static_cast<IMeshAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

        /// Get the mesh asset
        /// \return
        virtual Data::IMeshAsset* GetMeshAsset() = 0;
    };
}