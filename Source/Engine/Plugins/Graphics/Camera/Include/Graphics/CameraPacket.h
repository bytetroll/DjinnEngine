//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Graphics/PacketBuffer.h>
#include <Graphics/PacketBatch.h>
#include <Math/Matrix4.h>

namespace Djinn::Graphics {
    class IOutput;

    struct CameraPacket : public IPacket {
        DJINN_COM_CLASSID();

        /// Flush this packet
        void Flush() override {
            output = nullptr;
            batch.Flush();
        }

        /// Output of this packet
        IOutput* output = nullptr;

        /// Transforms
        Math::Matrix4 view;
        Math::Matrix4 projection;

        /// Contained packets
        PacketBatch batch;
    };

    /// Buffer helper
    using CameraPacketBuffer = PacketObjectBuffer<CameraPacket>;
}