//
// Created by Dev on 8/30/2018.
//
#pragma once

#include <COM/ID.h>

namespace Djinn::Graphics {
    struct CameraPacket;
    class IOutput;
}

namespace Djinn::Game {
    class ICamera {
    public:
        DJINN_COM_INTERFACEID();

        /// Build a camera packet
        /// \param packet
        virtual void Build(Graphics::CameraPacket* packet) = 0;

        /// Get the actual output
        /// \return
        virtual Graphics::IOutput* GetOutput() = 0;
    };
}