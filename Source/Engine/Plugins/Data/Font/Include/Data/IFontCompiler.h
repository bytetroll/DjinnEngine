//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Data/IAssetCompiler.h>
#include "FontConfiguration.h"

namespace Djinn::Data {
    class IFontAsset;

    class DJINN_EXPORT_PLUGINS_DATA_FONT IFontCompiler : public IAssetCompiler {
    public:
        DJINN_COM_INTERFACE();

        IFontCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompiler(registry, outer, host) {

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
                case IFontCompiler::kIID:
                    *out = static_cast<IFontCompiler*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}