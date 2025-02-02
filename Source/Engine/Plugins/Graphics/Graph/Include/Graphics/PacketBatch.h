//
// Created by Dev on 2/27/2018.
//
#pragma once

#include <COM/ID.h>
#include <Core/HashMap.h>
#include <Platform/Mutex.h>
#include <Platform/LockGuard.h>
#include "IPacket.h"

namespace Djinn::Graphics {
    class PacketBatch {
    public:
        DJINN_COM_CLASSID();

        /// Get a packet, will create if not present
        /// \tparam T
        /// \return
        template<typename T>
        T *GetPacket() {
            Platform::LockGuard guard(mtx);
            return Ensure<T>();
        }

        /// Flush this batch
        void Flush() {
            Platform::LockGuard guard(mtx);
            for (auto kv : packets) {
                kv.second->Flush();
            }
        }

    private:
        template<typename T>
        T* Ensure() {
            auto& packet = packets[T::kCID];
            if (!packet) {
                packet = new T();
            }
            return reinterpret_cast<T*>(packet);
        }

        Platform::SpinMutex mtx;
        Core::HashMap<COM::ClassID, IPacket*> packets;
    };
}