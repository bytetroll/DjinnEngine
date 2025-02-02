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
 on 9/12/17.
//

#pragma once

#include <Core/Size.h>
#include <Core/Annotation.h>
#include "fcontext.h"
#include "Config.h"

namespace Djinn::Async::Detail {
    struct StackControl {
        StackControl() = default;

        StackControl(UInt32 stackSize) {
            // Allocate blob
            void* blob = (void*)new char[stackSize + sizeof(Control)];

            // Control is the bottom offset
            control = new (blob) Control();

            // Set control data
            control->stackSize = stackSize;
            control->stackPtr = (void*)((char*)blob + sizeof(Control));
        }

        StackControl(void* ptr) {
            control = (Control*)ptr;
        }

        void* GetStackPtr() {
            return control->stackPtr;
        }

        void* GetStackTopPtr() {
            return (void*)((char*)control->stackPtr + control->stackSize);
        }

        USize GetStackSize() {
            return (USize)control->stackSize;
        }

        void SetUserData(void* data) {
            control->userData = data;
        }

        void SetContext(fcontext_t context) {
            control->context = context;
        }

        void* GetContext() {
            return control->context;
        }

        void* GetUserData() {
            return control->userData;
        }

        void SetHandle(void(*handle)(void*)) {
            control->handle = handle;
        }

        Attr(Ignore)
        void(*GetHandle())(void*) {
            return control->handle;
        }

        bool IsValid() const {
            return control != nullptr;
        }

        void* GetPtr() {
            return control;
        }

    private:
        struct Control {
            // 32
            UInt32 stackSize = 0;

            // 8, 40
            void* stackPtr = nullptr;

            // 8, 48
            void* userData = nullptr;

            // 8, 56
            fcontext_t context = nullptr;

            // 8, 64
            void(*handle)(void*) = nullptr;
        };

        Control* control = nullptr;
    };
}