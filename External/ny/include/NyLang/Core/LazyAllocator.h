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
 on 3/24/2018.
//
#pragma once

#include <cstdint>
#include <cstdlib>
#include <algorithm>

namespace NyLang {
    struct DefaultGrower {
        std::size_t operator()(std::size_t offset) {
            return offset * 2 + 64000;
        }
    };

    template<typename GROWER = DefaultGrower>
    class LazyAllocator {
    public:
        LazyAllocator() {
            AppendBlock(1);
        }

        ~LazyAllocator() {
            for (Block *it = head; it && it != tail;) {
                /*for (std::size_t i = 0; i < it->size; i++) {
                    it->elements[i].~T();
                }*/
                auto next = it->next;
                free(it);
                it = next;
            }
            for (std::size_t i = 0; i < blockOffset; i++) {
                //tail->elements[i].~T();
            }
            free(tail);
        }

        template<typename T>
        T *AllocateUninitialized() {
            std::size_t next = blockOffset + sizeof(T);
            if (next >= tail->size) {
                AppendBlock(sizeof(T));
            }

            size += sizeof(T);
            auto ptr = &tail->elements[blockOffset];
            blockOffset = next;
            return reinterpret_cast<T*>(ptr);
        }

        template<typename T>
        T *AllocateUninitialized(std::size_t count) {
            std::size_t next = blockOffset + sizeof(T) * count;
            if (next >= tail->size) {
                AppendBlock(sizeof(T) * count);
            }

            size += sizeof(T) * count;
            auto ptr = &tail->elements[blockOffset];
            blockOffset = next;
            return reinterpret_cast<T*>(ptr);
        }

    private:
        struct Block {
            Block *next;
            std::size_t size;
            char elements[];
        };

        void AppendBlock(std::size_t min) {
            auto blockSize = std::max(min, GROWER{}(size));
            auto block = reinterpret_cast<Block *>(malloc(sizeof(Block) + blockSize));
            block->next = nullptr;
            block->size = blockSize;
            blockOffset = 0;

            if (!head) {
                head = tail = block;
            } else {
                tail->next = block;
                tail = block;
            }
        }

        std::size_t size = 0;
        std::size_t blockOffset = 0;
        Block *head = nullptr;
        Block *tail = nullptr;
    };
}