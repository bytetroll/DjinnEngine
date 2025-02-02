//
// Created by Dev on 3/24/2018.
//
#pragma once

#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <Core/Size.h>

namespace Djinn::Memory {
    struct DefaultGrower {
        USize operator()(USize offset) {
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
                /*for (USize i = 0; i < it->size; i++) {
                    it->elements[i].~T();
                }*/
                auto next = it->next;
                free(it);
                it = next;
            }
            for (USize i = 0; i < blockOffset; i++) {
                //tail->elements[i].~T();
            }
            free(tail);
        }

        template<typename T>
        T *AllocateUninitialized() {
            USize next = blockOffset + sizeof(T);
            if (next >= tail->size) {
                AppendBlock(sizeof(T));
            }

            size += sizeof(T);
            auto ptr = &tail->elements[blockOffset];
            blockOffset = next;
            return reinterpret_cast<T*>(ptr);
        }

        template<typename T>
        T *AllocateUninitialized(USize count) {
            USize next = blockOffset + sizeof(T) * count;
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
            USize size;
            char elements[];
        };

        void AppendBlock(USize min) {
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

        USize size = 0;
        USize blockOffset = 0;
        Block *head = nullptr;
        Block *tail = nullptr;
    };
}