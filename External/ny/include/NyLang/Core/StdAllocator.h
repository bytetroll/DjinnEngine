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
 on 11/14/2017.
//
#pragma once

#include <cstddef>
#include <new>
#include <memory>
#include <limits>
#include "SimpleAllocator.h"

namespace NyLang {
    template<typename T>
    class StdAllocator {
    public:
        typedef T value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

    public:
        template<typename U>
        struct rebind {
            typedef StdAllocator<U> other;
        };

    public:
        inline explicit StdAllocator() {

        }

        inline ~StdAllocator() {

        }

        inline StdAllocator(StdAllocator const &) {

        }

        template<typename U>
        inline StdAllocator(StdAllocator<U> const &) {

        }

        inline pointer address(reference r) {
            return &r;
        }

        inline const_pointer address(const_reference r) {
            return &r;
        }

        inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = nullptr) {
            return reinterpret_cast<pointer>(::operator new(cnt * sizeof(T)));
            //return reinterpret_cast<pointer>(SimpleAllocator::Allocate(cnt * sizeof(T)));
        }

        inline void deallocate(pointer p, size_type) {
            ::operator delete(p);
            //SimpleAllocator::Free(p);
        }

        inline size_type max_size() const {
            return std::numeric_limits<size_type>::max() / sizeof(T);
        }

        inline void construct(pointer p, const T &t) {
            new(p) T(t);
        }

        inline void destroy(pointer p) {
            p->~T();
        }

        inline bool operator==(StdAllocator const &) {
            return true;
        }

        inline bool operator!=(StdAllocator const &a) {
            return !operator==(a);
        }
    };
}