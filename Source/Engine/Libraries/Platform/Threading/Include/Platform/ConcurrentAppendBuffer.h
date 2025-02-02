//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Array.h>
#include <Core/Memory.h>
#include <Core/Atomic.h>
#include <Core/List.h>
#include <Core/SparseArray.h>
#include <Core/Buffer.h>
#include "Thread.h"
#include "ConcurrentSparseArray.h"

namespace Djinn::Platform {
    template<typename T>
    class ConcurrentAppendBuffer {
    public:
        /// Append multiple values
        void Add(UInt32 count, const T *ptr) {
            auto threadBuffer = GetThreadBuffer();
            threadBuffer->Resize(threadBuffer->Size() + count);
            T* base = threadBuffer->Ptr() + threadBuffer->Size() - count;
            Core::Memory::Copy(base, ptr, sizeof(T) * count);
        }

        /// Append a single value
        void Add(const T &buffer) {
            Add(1, &buffer);
        }

        /// Combine all buffers, must be called before GetBuffer
        void Combine() {
            LockGuard guard(mtx);
            buffer.Clear();
            for (auto object : objects) {
                buffer.Add(*object);
            }
        }

        /// Clear this buffer and all working buffers
        void Clear() {
            LockGuard guard(mtx);
            buffer.Clear();
            for (auto object : objects) {
                object->Clear();
            }
        }

        /// Get backend buffer, will not combine result
        /// \return
        Core::Buffer<T> &GetBuffer() {
            return buffer;
        }

    private:
        Core::Buffer<T> *GetThreadBuffer() {
            bool created;
            auto *buffer = &sparseLUT.At(ThreadAPI::GetCurrentID(), created);
            if (created) {
                LockGuard guard(mtx);
                objects.Add(buffer);
            }
            return buffer;
        }

        Core::Buffer<T> buffer;
        ConcurrentSparseArray<Core::Buffer<T>> sparseLUT;
        SpinMutex mtx;
        Core::Buffer<Core::Buffer<T> *> objects;
    };
}