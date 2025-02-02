//
// Created by Dev on 1/21/2019.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Game {
    class ISceneState : public COM::TUnknown<ISceneState> {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        ISceneState(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }
    };
}