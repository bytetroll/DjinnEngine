//
// Created by Dev on 2/24/2018.
//
#pragma once

#include <Graphics/IPass.h>

namespace Djinn::Graphics {
    class ILightingPass : public IPass {
    public:
        DJINN_COM_INTERFACE();

        ILightingPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPass(registry, outer, host) {

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
                case ILightingPass::kIID:
                    *out = static_cast<ILightingPass*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}