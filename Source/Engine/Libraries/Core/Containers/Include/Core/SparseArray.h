//
// Created by Dev on 1/14/2018.
//
#pragma once

#include "Array.h"
#include <Core/Literals.h>
#include <Core/Math.h>
#include <Core/Pair.h>

namespace Djinn::Core {
    template<typename T, USize BLOCK_COUNT = 1024>
    class SparseArray {
        struct Block {
            Core::Pair<bool, T> elements[BLOCK_COUNT];
        };

    public:
        struct Iterator {
            Iterator(typename Core::Array<Block *>::Iterator start, typename Core::Array<Block *>::Iterator end) : block(start), blockEnd(end) {
                while (block != blockEnd && !*block) {
                    ++block;
                }
            }

            Iterator &operator++() {
                if (block != blockEnd) {
                    while (++index < BLOCK_COUNT && !(*block)->elements[index].first);
                    if (index >= BLOCK_COUNT) {
                        index = 0;
                        ++block;
                        while (block != blockEnd && !*block) {
                            ++block;
                        }
                    }
                }
                return *this;
            }

            T *operator->() {
                return &(*block)->elements[index].second;
            }

            T &operator*() {
                return (*block)->elements[index].second;
            }

            T *operator->() const {
                return &(*block)->elements[index].second;
            }

            T &operator*() const {
                return (*block)->elements[index].second;
            }

            operator bool() {
                return block != blockEnd;
            }

            USize index = 0;
            typename Core::Array<Block *>::Iterator block;
            typename Core::Array<Block *>::Iterator blockEnd;
        };

        struct ReverseIterator {
            ReverseIterator(typename Core::Array<Block *>::ReverseIterator start, typename Core::Array<Block *>::ReverseIterator end) : block(start), blockEnd(end) {
                while (block != blockEnd && !*block) {
                    ++block;
                }
                if (block != blockEnd) {
                    while (!(*block)->elements[index].first && index > 0) index--;
                }
            }

            ReverseIterator &operator++() {
                if (block != blockEnd) {
                    while (--index >= 0 && !(*block)->elements[index].first);
                    if (index <= -1) {
                        index = BLOCK_COUNT - 1;
                        ++block;
                        while (block != blockEnd && !*block) {
                            ++block;
                        }
                        if (block != blockEnd) {
                            while (--index >= 0 && !(*block)->elements[index].first);
                        }
                    }
                }
                return *this;
            }

            T *operator->() {
                return &(*block)->elements[index].second;
            }

            T &operator*() {
                return (*block)->elements[index].second;
            }

            T *operator->() const {
                return &(*block)->elements[index].second;
            }

            T &operator*() const {
                return (*block)->elements[index].second;
            }

            operator bool() {
                return block != blockEnd;
            }

            Int64 index = BLOCK_COUNT - 1;
            typename Core::Array<Block *>::ReverseIterator block;
            typename Core::Array<Block *>::ReverseIterator blockEnd;
        };

        /// Get element
        /// \param i
        /// \return
        T &operator[](USize i) {
            return At(i);
        }

        /// Get element
        /// \param i
        /// \return
        T &At(USize i) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            EnsureBlock(block);
            auto &element = blocks[block]->elements[i - block * BLOCK_COUNT];
            if (!element.first) {
                element.second = {};
                element.first = true;
                size++;
            }
            return element.second;
        }

        /// Get element
        /// \param i
        /// \return
        T &At(USize i, bool &created) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            EnsureBlock(block);
            auto &element = blocks[block]->elements[i - block * BLOCK_COUNT];
            if (!element.first) {
                element.second = {};
                element.first = true;
                created = true;
                size++;
            } else {
                created = false;
            }
            return element.second;
        }

        /// Remove an element
        /// \param i
        void Remove(USize i) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            EnsureBlock(block);
            auto &element = blocks[block]->elements[i - block * BLOCK_COUNT];
            element.first = false;
            element.second = {};
        }

        /// Get element
        /// \param i
        /// \return
        bool Get(USize i, T &out) {
            USize block = static_cast<USize>(Math::Floor(static_cast<double>(i) / BLOCK_COUNT));
            if (block >= blocks.Size() || blocks[block] == nullptr) {
                return false;
            }
            auto &element = blocks[block]->elements[i - block * BLOCK_COUNT];
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

        void Clear() {
            for (auto block : blocks) {
                if (block) {
                    for (USize i = 0; i < BLOCK_COUNT; i++) {
                        block->elements[i].first = false;
                    }
                }
            }
        }

        Iterator It() {
            return Iterator(blocks.Start(), blocks.End());
        }

        ReverseIterator RIt() {
            return ReverseIterator(blocks.RStart(), blocks.REnd());
        }

    private:
        void EnsureBlock(USize block) {
            blocks.ResizePassive(block + 1);
            if (!blocks[block]) {
                blocks[block] = new Block();
            }
        }

        USize size = 0;
        Core::Array<Block *> blocks;
    };
}