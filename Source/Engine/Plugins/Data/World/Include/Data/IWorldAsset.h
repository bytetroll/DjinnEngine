//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Data/IAsset.h>

namespace Djinn::Data {
    class WorldConfig;

    class DJINN_EXPORT_PLUGINS_DATA_WORLD IWorldAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        IWorldAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

        }

        /// Get configuration
        /// \return
        virtual WorldConfig* GetConfig() = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAsset::kIID:
                    *out = static_cast<IAsset*>(this);
                    return COM::kOK;
                case IWorldAsset::kIID:
                    *out = static_cast<IWorldAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}