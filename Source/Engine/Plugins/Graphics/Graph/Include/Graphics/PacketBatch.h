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