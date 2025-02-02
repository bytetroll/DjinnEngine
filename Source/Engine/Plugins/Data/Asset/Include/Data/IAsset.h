//
// Created by Dev on 9/4/2017.
//
#pragma once

#include <COM/ID.h>
#include <COM/IUnknown.h>

namespace Djinn::Data {
    class IAsset : public COM::TUnknown<IAsset> {
    public:
        DJINN_COM_INTERFACE();

        IAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }
    };
}