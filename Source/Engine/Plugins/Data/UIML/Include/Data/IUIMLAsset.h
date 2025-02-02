//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Data/IAsset.h>

namespace Djinn::Data {
    struct UIMLContent;

    class DJINN_EXPORT_PLUGINS_DATA_UIML IUIMLAsset : public IAsset {
    public:
        DJINN_COM_INTERFACE();

        IUIMLAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAsset(registry, outer, host) {

        }

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IAsset::kIID:
                    *out = static_cast<IAsset*>(this);
                    return COM::kOK;
                case IUIMLAsset::kIID:
                    *out = static_cast<IUIMLAsset*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

        /// Get content
        /// \return
        virtual UIMLContent* GetContent() = 0;
    };
}