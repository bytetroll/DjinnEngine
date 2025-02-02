//
// Created by Dev on 8/25/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Game {
    class ISceneOcclusion : public COM::TUnknown<ISceneOcclusion> {
    public:
        DJINN_COM_INTERFACE();

        ISceneOcclusion(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }


    };
}
