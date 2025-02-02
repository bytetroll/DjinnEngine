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
 on 1/14/2018.
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