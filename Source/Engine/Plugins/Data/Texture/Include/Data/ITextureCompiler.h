//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Data/IAssetCompiler.h>
#include "TextureConfiguration.h"

namespace Djinn::Data {
    class ITextureAsset;

    class DJINN_EXPORT_PLUGINS_DATA_TEXTURE ITextureCompiler : public IAssetCompiler {
    public:
        DJINN_COM_INTERFACE();

        ITextureCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetCompiler(registry, outer, host) {

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
                case ITextureCompiler::kIID:
                    *out = static_cast<ITextureCompiler*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}