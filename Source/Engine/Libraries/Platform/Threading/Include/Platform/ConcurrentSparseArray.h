//
// Created by Dev on 1/14/2018.
//
#pragma once

#include <Core/Literals.h>
#include <Core/Math.h>
#include <Core/Pair.h>
#include <Core/Array.h>
#include <Core/Atomic.h>
#include "SpinMutex.h"
#include "LockGuard.h"

namespace Djinn::Platform {
    template<typename T, USize BLOCK_COUNT = 1024>
    class ConcurrentSparseArray {
        struct Block {
            Core::Pair<bool, T> elements[BLOCK_COUNT];
        };

    public:
        /// Get element
        /// \param i
        /// \return
        T& operator[](USize i) {
            return At(i);
        }

        /// Get element
        /// \param i
        /// \return
        T& At(USize i) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            EnsureBlock(block);
            auto& element = blocks[block]->elements[i - block * BLOCK_COUNT];
            element.first = true;
            return element.second;
        }

        /// Get element
        /// \param i
        /// \return
        T& At(USize i, bool& created) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            EnsureBlock(block);
            auto& element = blocks[block]->elements[i - block * BLOCK_COUNT];
            created = (element.first == false);
            element.first = true;
            return element.second;
        }

        /// Get element
        /// \param i
        /// \return
        bool Get(USize i, T& out) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            if (block >= blocks.Size() || blocks[block] == nullptr) {
                return false;
            }
            auto& element = blocks[block]->elements[i - block * BLOCK_COUNT];
            if (!element.first) {
                return false;
            }
            out = element.second;
            return true;
        }

        /// Get element
        /// \param i
        /// \return
        bool Contains(USize i) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            return block < blocks.Size() && blocks[block] != nullptr && blocks[block]->elements[i - block * BLOCK_COUNT].first;
        }

        /// Get size
        /// \return
        USize Size() const {
            return size;
        }

    private:
        void EnsureBlock(USize block) {
            if (static_cast<USize>(blockCount.Load()) <= block) {
                LockGuard guard(mutex);
                if (static_cast<USize>(blockCount.Load()) <= block) {
                    blocks.Resize(block + 1);
                    blockCount = block + 1;
                }
            }
            if (!blocks[block]) {
                LockGuard guard(mutex);
                if (!blocks[block]) {
                    blocks[block] = new Block();
                }
            }
        }

        SpinMutex mutex;
        USize size = 0;

        Core::AtomicCounter blockCount{0};
        Core::Array<Block*> blocks;
    };
}