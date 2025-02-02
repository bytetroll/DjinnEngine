//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <UI/IControl.h>

namespace Djinn::Editor {
    class ITemplates : public UI::IControl {
    public:
        DJINN_COM_INTERFACE();

        ITemplates(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IControl(registry, outer, host) {

        }
    };
}