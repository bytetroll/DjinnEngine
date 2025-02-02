//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Memory.h>
#include <Core/Atomic.h>
#include <Core/SparseArray.h>
#include <Platform/Mutex.h>
#include "Thread.h"

namespace Djinn::Platform {
    template<typename T, Int64 BLOCK_SIZE>
    class ConcurrentStorageBuffer {
    public:
        ConcurrentStorageBuffer() {
            block = baseBlock;
        }

        /// Get storage for multiple values
        T* Add(UInt32 count, const T *ptr) {
            DJINN_ASSERT(count <= BLOCK_SIZE, "Append size exceeded BLOCK_SIZE for ConcurrentStorageBuffer");
            T* base = GetAlloc(count);
            Core::Memory::Copy(base, ptr, sizeof(T) * count);
            return base;
        }

        /// Append a single value
        T* Add(const T &buffer) {
            return Add(1, &buffer);
        }

        /// Clear this buffer and all working buffers
        void Clear() {
            offset = 0;
            block = baseBlock;
        }

    private:
        struct Block {
            T data[BLOCK_SIZE];
            Block* next = nullptr;
        };

        T* GetAlloc(UInt32 count) {
            Int64 _offset = offset.FetchAdd(count);
            if ((_offset + count) >= BLOCK_SIZE) {
                Platform::LockGuard guard(mtx);
                if (offset.Load() != 0) {
                    if (!block->next) {
                        block->next = new Block();
                    }
                    block = block->next;
                    offset = 0;
                }
                _offset = 0;
            }


            return &block->data[_offset];
        }

        Platform::SpinMutex mtx;
        Block* baseBlock = new Block();
        Block* block = nullptr;
        Core::AtomicCounter offset{0};
    };
}