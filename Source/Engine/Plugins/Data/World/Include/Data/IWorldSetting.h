//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Data {
    class IWorldSetting : public COM::TUnknown<IWorldSetting> {
    public:
        DJINN_COM_INTERFACE();

        IWorldSetting(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }
    };
}