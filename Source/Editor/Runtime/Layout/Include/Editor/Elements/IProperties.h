//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <UI/IControl.h>

namespace Djinn::Editor {
    class IProperties : public UI::IControl {
    public:
        DJINN_COM_INTERFACE();

        IProperties(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

        }
    };
}