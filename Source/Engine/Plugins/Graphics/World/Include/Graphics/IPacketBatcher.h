//
// Created by Dev on 3/1/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Async/Async.h>

namespace Djinn::Graphics {
    struct WorldPacket;

    class IPacketBatcher {
    public:
        DJINN_COM_INTERFACEID();

        /// Build onto the supplied world packet
        /// \param batch
        /// \return
        virtual COM::Result Build(WorldPacket* batch) = 0;

        /// Asynchronous methods
        Asyncf(Build);
    };
}