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
 on 5/5/2018.
//
#pragma once

#include "LazyAllocator.h"

namespace NyLang {
    class Context {
    public:
        ~Context() {

        }

        /// Create new from instance
        /// \tparam T
        /// \param from
        /// \return
        /*template<typename T>
        T* New(const T& from) {
            auto ptr = allocator.AllocateUninitialized<T>();
            new (ptr) T(from);
            return ptr;
        }*/

        /// Create new from instance
        /// \tparam T
        /// \param from
        /// \return
        template<typename T, typename... A>
        T* New(A&&... args) {
            auto ptr = allocator.AllocateUninitialized<T>();
            new (ptr) T(args...);
            return ptr;
        }

        /// Create new to destination
        /// \tparam T
        /// \param to
        /// \return
        /*template<typename T>
        T* New(T*& to) {
            new (to = allocator.AllocateUninitialized<T>()) T();
            return to;
        }*/

        /// Create new by array
        /// \tparam T
        /// \param count
        /// \return
        template<typename T>
        T* NewArray(std::size_t count) {
            auto ptr = allocator.AllocateUninitialized<T>(count);
            return ptr;
        }

    private:
        LazyAllocator<> allocator;
    };
}