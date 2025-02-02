//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 2/27/2018.
//
#pragma once

#include <COM/ID.h>
#include <Core/Array.h>
#include <Graphics/IPacket.h>
#include <Platform/ConcurrentAppendBuffer.h>
#include <Core/Pool.h>

namespace Djinn::Graphics {
    /// Packet buffer helper
    /// \tparam T
    template<typename T>
    class PacketBuffer : public IPacket {
    public:
        DJINN_COM_CLASSID();

        /// Add a packet
        /// \param packet
        void Add(const T& packet) {
            Platform::LockGuard guard(mutex);
            packets.Add(packet);
        }

        /// Flush a packet
        void Flush() override {
            packets.Clear();
        }

        /// Get all packets
        /// \return
        Core::Buffer<T>& GetPackets() {
            return packets;
        }

        /// Pop a temporary buffer
        /// \return
        Core::Buffer<T>* PopBuffer() {
            Platform::LockGuard guard(mutex);
            return bufferPool.Pop();
        }

        /// Push a temporary buffer
        /// \param buffer
        void PushBuffer(Core::Buffer<T>* buffer) {
            Platform::LockGuard guard(mutex);
            packets.Add(*buffer);
            buffer->Clear();
            bufferPool.Push(buffer);
        }

    private:
        Platform::SpinMutex mutex;
        Core::ObjectPool<Core::Buffer<T>> bufferPool;
        Core::Buffer<T> packets;
    };

    template<typename T>
    class PacketObjectBuffer : public IPacket {
    public:
        DJINN_COM_CLASSID();

        /// Add a new packet
        /// \return
        T* Add() {
            Platform::LockGuard guard(mtx);
            T* packet = packets.Pop();
            return packet;
        }

        /// Overrides
        virtual void Flush() override {
            Platform::LockGuard guard(mtx);
            for (auto packet : packets.GetObjects()) {
                packet->Flush();
            }
            packets.Flush();
        }

        /// Get all packets
        /// \return
        Core::Array<T*>& GetPackets() {
            return packets.GetObjects();
        }

    private:
        Platform::SpinMutex mtx;
        Core::LazyObjectPool<T> packets;
    };
}