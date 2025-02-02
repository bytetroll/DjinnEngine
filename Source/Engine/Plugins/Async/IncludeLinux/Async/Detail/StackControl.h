//
// Created by dev on 9/12/17.
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