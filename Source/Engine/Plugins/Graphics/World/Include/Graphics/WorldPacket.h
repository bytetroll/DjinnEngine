//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <Core/Types.h>
#include <Core/Delegate.h>
#include <Graphics/PacketBuffer.h>
#include <Graphics/PacketBatch.h>
#include <Core/Atomic.h>

namespace Djinn::Graphics {
    struct WorldPacket : public IPacket {
        DJINN_COM_CLASSID();

        /// Flush this packet
        void Flush() override {
            batch.Flush();
        }

        /// Release usage
        void Release() {
            if (deleter.IsValid()) {
                deleter();
                deleter = {};
            }
        }

        /// Contained packets
        PacketBatch batch;

        /// Deleter
        Core::Delegate<void()> deleter;
    };
}