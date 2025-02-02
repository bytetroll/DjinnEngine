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
#include "Async/ContextAPI.h"
#include <rpc.h>
#include <iostream>
#include <Core/Set.h>
#include <Platform/Mutex.h>
#include <Platform/LockGuard.h>
#include <Core/Assert.h>
#include <Async/Detail/Config.h>

DJINN_NS3(Async, API);

namespace {
    void* gUserData = nullptr;
}

void API::StartThreadContext(void(*handle)(ThreadContextData*), void* user) {
    ThreadContextData data = {};
    data.user = user;
    data.context = ConvertThreadToFiber(nullptr);

    handle(&data);
}

void API::SwitchToContext(Context context) {
    DJINN_ASSERT(context != nullptr, "Invalid context");

    SwitchToFiber(context);
}

Context API::GetCurrentContext() {
    return IsThreadAFiber() ? GetCurrentFiber() : nullptr;
}

void *API::GetContextData() {
    return IsThreadAFiber() ? GetFiberData() : gUserData;
}

Context API::CreateContext(void(*handle)(void *), void *data) {
    return CreateFiber(DJINN_ASYNC_STACK_SIZE, handle, data);
}

bool API::IsWithinContext() {
    return IsThreadAFiber();
}

void API::SetDefaultContextData(void *data) {
    gUserData = data;
}