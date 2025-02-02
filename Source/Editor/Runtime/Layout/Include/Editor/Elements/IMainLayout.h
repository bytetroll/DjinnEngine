//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <UI/IControl.h>

namespace Djinn::Editor {
    class ILevel;

    class IMainLayout : public UI::IControl {
    public:
        DJINN_COM_INTERFACE();

        IMainLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IControl(registry, outer, host) {

        }

        /// Add a level to this layout
        /// Will setup a level layout unique to that level
        /// \param level
        /// \return
        virtual COM::Result AddLevel(ILevel* level) = 0;
    };
}