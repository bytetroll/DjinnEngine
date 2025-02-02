//
// Created by Dev on 7/16/2018.
//
#pragma once

#include <Data/IAsset.h>

namespace Djinn::Data {
    class ITextureAsset;
}

namespace Djinn::Editor {
    class IAsset : public Data::IAsset {
    public:
        DJINN_COM_INTERFACE();

        IAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : Data::IAsset(registry, outer, host) {

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
            }
            return COM::kNotImplemented;
        }

        /// Get the thumbnail of this asset
        /// \return
        virtual Data::ITextureAsset* GetThumbnail() = 0;
    };
}