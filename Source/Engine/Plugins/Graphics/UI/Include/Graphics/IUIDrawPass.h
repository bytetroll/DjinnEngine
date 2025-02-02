//
// Created by Dev on 2/24/2018.
//
#pragma once

#include "IUIPass.h"

namespace Djinn::Graphics {
    class DJINN_EXPORT_PLUGINS_GRAPHICS_UI IUIDrawPass : public IUIPass {
    public:
        DJINN_COM_INTERFACE();

        IUIDrawPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIPass(registry, outer, host) {

        }

        /// Overrides
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<COM::IUnknown*>(this);
                    return COM::kOK;
                case IPass::kIID:
                    *out = static_cast<IPass*>(this);
                    return COM::kOK;
                case IUIPass::kIID:
                    *out = static_cast<IUIPass*>(this);
                    return COM::kOK;
                case IUIDrawPass::kIID:
                    *out = static_cast<IUIDrawPass*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}