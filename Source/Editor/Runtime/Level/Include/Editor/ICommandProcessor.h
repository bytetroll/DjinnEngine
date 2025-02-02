//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Editor {
    class ICommandHost;

    class ICommandProcessor {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the command worldHost of this processor
        /// \return
        virtual ICommandHost* GetCommandHost() = 0;
    };
}