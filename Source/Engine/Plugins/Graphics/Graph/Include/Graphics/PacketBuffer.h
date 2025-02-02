//
// Created by Dev on 2/27/2018.
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