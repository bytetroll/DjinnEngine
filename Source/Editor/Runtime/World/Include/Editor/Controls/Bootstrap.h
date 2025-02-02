//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <UI/UIMLAnnotation.h>
#include <UI/IControl.h>

namespace Djinn::Editor {
    class Bootstrap : public UI::IControl {
    public:
        DJINN_COM_CLASS();

        Bootstrap(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
    };
}
