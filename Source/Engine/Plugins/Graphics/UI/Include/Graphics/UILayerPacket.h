//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Graphics/PacketBuffer.h>
#include <Graphics/PacketBatch.h>

namespace Djinn::Graphics {
    struct UILayerPacket : public IPacket {
        DJINN_COM_CLASSID();

        /// Flush this packet
        void Flush() override {
            batch.Flush();
            depth = 0;
        }

        /// Contained packets
        PacketBatch batch;

        /// Z / Depth offset of this window
        UInt64 depth = 0;
    };

    /// Buffer helper
    using UILayerPacketBuffer = PacketObjectBuffer<UILayerPacket>;
}