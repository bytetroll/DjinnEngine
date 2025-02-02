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
 on 8/9/2017.
//

#include <Core/Common.h>
#include <Async/ContextAPI.h>
#include <iostream>
#include <Core/Set.h>
#include <Platform/Mutex.h>
#include <Platform/LockGuard.h>
#include <Core/Assert.h>
#include <Core/Functional.h>
#include <Async/Detail/Config.h>
#include <Platform/Thread.h>

DJINN_NS3(Async, API);

struct AsyncControl {
    void *GetStackTopPtr() {
        return static_cast<void *>(static_cast<char *>(stackPtr) + stackSize);
    }

    // 32
    UInt32 stackSize = 0;

    // 8, 40
    void *stackPtr = nullptr;

    // 8, 48
    void *userData = nullptr;

    // 8, 56
    Detail::fcontext_t fctx = nullptr;

    // 8, 64
    void (*handle)(void *) = nullptr;
};

/*namespace {*/
    bool IsUpStack(volatile char *function_parameter) {
        volatile char local [64];
        if (&local [63] > function_parameter)
            return true;
        else
            return false;
    }

    AsyncControl *AllocControlBlock(void(*handle)(void *) = nullptr, void *data = nullptr) {
        // Allocate blob
        void *blob = new char[DJINN_ASYNC_STACK_SIZE + sizeof(AsyncControl)];

        // Control is the bottom offset
        AsyncControl *control = new(blob) AsyncControl();

        // Determine stack pointer
        auto ptr = blob;
        //if (!IsUpStack(nullptr)) {
            ptr = static_cast<void *>(static_cast<char *>(blob) + sizeof(AsyncControl));
        //}

        // Set control data
        control->stackSize = DJINN_ASYNC_STACK_SIZE;
        control->stackPtr = ptr;
        control->handle = handle;
        control->userData = data;
        return control;
    }

    thread_local AsyncControl someCOntrol;
    thread_local AsyncControl *gCurrentControl = nullptr;
    thread_local AsyncControl *gLastControl = nullptr;

    void* gUserData = nullptr;
//}

void API::StartThreadContext(void(*handle)(ThreadContextData *), void *user) {
    // Create context
    ThreadContextData data = {};
    data.user = user;
    data.context = CreateContext(reinterpret_cast<void (*)(void *)>(handle), &data);

    // Jump to context
    SwitchToContext(data.context);
}

void API::SwitchToContext(Context context) {
    DJINN_ASSERT(context != nullptr, "Invalid context");

    // ...
    auto control = static_cast<AsyncControl *>(context);
    DJINN_ASSERT(control->fctx != nullptr, "Context {fcontext} invalid, state was not updated");

    // Exchange current control
    gLastControl = gCurrentControl;
    AsyncControl *last = Core::Exchange(gCurrentControl, control);
    someCOntrol = *gCurrentControl;

    // Switch
    Detail::transfer_t tr = Detail::jump_fcontext(Core::Exchange(control->fctx, nullptr), control);

    // Transfer dictates new context of control
    control->fctx = tr.fctx;
}

Context API::GetCurrentContext() {
    return static_cast<Context>(gCurrentControl);
}

void *API::GetContextData() {
    return gCurrentControl ? gCurrentControl->userData : gUserData;
}

Context API::CreateContext(void(*handle)(void *), void *data) {
    // Allocate control structure
    AsyncControl *newControl = AllocControlBlock(handle, data);

    // ...
    newControl->fctx = Detail::make_fcontext(newControl->GetStackTopPtr(), newControl->stackSize, [](Detail::transfer_t tr) {
        if (gLastControl) {
            gLastControl->fctx = tr.fctx;
        }

        // Get control
        auto&& control = static_cast<AsyncControl*>(tr.data);
        // Launch user operation
        control->handle(control->userData);
    });

    // ...
    return static_cast<Context>(newControl);
}

bool API::IsWithinContext() {
    return gCurrentControl != nullptr;
}

void API::SetDefaultContextData(void *data) {
    gUserData = data;
}
