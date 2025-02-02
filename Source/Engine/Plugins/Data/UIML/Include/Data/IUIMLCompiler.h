//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Data/IAssetCompiler.h>

namespace Djinn::Data {
    class IUIMLAsset;

    class DJINN_EXPORT_PLUGINS_DATA_UIML IUIMLCompiler : public IAssetCompiler {
    public:
        DJINN_COM_INTERFACE();

        IUIMLCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompiler(registry, outer, host) {

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
                case IUIMLCompiler::kIID:
                    *out = static_cast<IUIMLCompiler*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}