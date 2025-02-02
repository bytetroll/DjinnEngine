//
// Created by Dev on 11/2/2018.
//
#pragma once

#include "IPlugin.h"

namespace Djinn::Host {
    class Plugin : public IPlugin {
    public:
        DJINN_COM_CLASS();

        Plugin(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);
    };
}