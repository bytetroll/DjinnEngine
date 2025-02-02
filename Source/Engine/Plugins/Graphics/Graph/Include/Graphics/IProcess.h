//
// Created by Dev on 2/24/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Graphics {
    class IPipeline;

    class IProcess : public COM::TUnknown<IProcess> {
    public:
        DJINN_COM_INTERFACE();

        IProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        virtual Graphics::IPipeline *GetPipeline() = 0;
    };
}