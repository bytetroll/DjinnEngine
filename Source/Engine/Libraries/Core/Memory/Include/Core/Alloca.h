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
 on 11/12/2018.
//
#pragma once

#include <Core/Size.h>
#include <Core/Common.h>
#include <Core/Assert.h>
#include <memory>

namespace Djinn::Memory {
    template<typename T>
    struct Alloca {
        DJINN_NOCOPY(Alloca);

        Alloca(T* ptr) : ptr(ptr) {

        }

        ~Alloca() {
            ptr->~T();
        }

        T* operator->() {
            return ptr;
        }

        operator T*() {
            return ptr;
        }

        T* ptr;
    };

    template<typename T>
    struct AllocaArray {
        DJINN_NOCOPY(AllocaArray);

        AllocaArray(T* ptr, USize size) : ptr(ptr), size(size) {

        }

        ~AllocaArray() {
            for (USize i = 0; i < size; i++) {
                ptr[i].~T();
            }
        }

        T& operator[](USize i) {
            DJINN_ASSERT(i < size, "Stack array index out of bounds");
            return ptr[i];
        }

        operator T*() {
            return ptr;
        }

        T* ptr;
        USize size;
    };

    /// Perform a stack allocation
#define DJINN_ALLOCA(TYPE, ...) Djinn::Memory::Alloca<TYPE>(new (_alloca(TYPE)) TYPE(__VA_ARGS__))

    /// Perform a stack array allocation
#define DJINN_ALLOCA_ARRAY(TYPE, COUNT, ...) Djinn::Memory::AllocaArray<TYPE>(new (_alloca(sizeof(TYPE))) TYPE[COUNT] __VA_ARGS__, COUNT)
}