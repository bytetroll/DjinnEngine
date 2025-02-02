//
// Created by Dev on 8/29/2018.
//
#pragma once

#include "ICameraRenderer.h"

namespace Djinn::Game {
    class CameraRenderer : public ICameraRenderer {
    public:
        DJINN_COM_CLASSID();

        /// Override
        COM::ClassID GetClassID() override {
            return kCID;
        }

        /// Override
        Graphics::CameraPacket *GetPacket() override {
            return packet;
        }

        /// Packet
        Graphics::CameraPacket* packet = nullptr;
    };
}