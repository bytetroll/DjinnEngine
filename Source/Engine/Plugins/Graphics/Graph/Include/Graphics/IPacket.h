//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <COM/ID.h>

namespace Djinn::Graphics {
    class IPacket {
    public:
        DJINN_COM_INTERFACEID();

        /// Flush this packet
        virtual void Flush() = 0;
    };
}