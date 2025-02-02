//
// Created by Dev on 8/9/2017.
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