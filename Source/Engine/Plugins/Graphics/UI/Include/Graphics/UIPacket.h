//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Graphics/PacketBuffer.h>
#include <Graphics/PacketBatch.h>

namespace Djinn::Graphics {
    class IOutput;

    struct UIPacket : public IPacket {
        DJINN_COM_CLASSID();

        /// Flush this packet
        void Flush() override {
            output = nullptr;
            batch.Flush();
        }

        /// Output of this packet
        IOutput* output = nullptr;

        /// Contained packets
        PacketBatch batch;
    };

    /// Packet helper
    using UIPacketBuffer = PacketObjectBuffer<UIPacket>;
}