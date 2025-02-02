//
// Created by Dev on 11/2/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Host {
    class IPlugin : public COM::TUnknown<IPlugin> {
    public:
        DJINN_COM_INTERFACE();

        IPlugin(Host::IRegistry *registry, IUnknown *outer, COM::IInstanceHost *host) : TUnknown(registry, outer, host) {

        }
    };
}