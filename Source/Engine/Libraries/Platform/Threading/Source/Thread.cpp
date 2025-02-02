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
 on 8/29/2016.
//

#include <Core/Assert.h>
#include <Core/CrashHandler.h>
#include "Platform/Thread.h"

DJINN_NS2(Platform);


Thread::Thread() : state(ThreadState::eIdle) {
    // ...
}

bool Thread::CanJoin() {
    dataMutex.Lock();
    bool result = state == ThreadState::eRunning;
    dataMutex.Unlock();
    return result;
}

bool Thread::Join() {
    if (CanJoin()) {
        ThreadAPI::Join(handle);
        return true;
    }
    return false;
}

void Thread::Detach() {
    dataMutex.Lock();
    if (state == ThreadState::eRunning) {
        ThreadAPI::Detach(handle);
        state = ThreadState::eIdle;
    }
    dataMutex.Unlock();
}

ThreadAPI::ThreadID Thread::GetID() {
    return ThreadAPI::GetHandleID(handle);
}

void Thread::Callback(void *_data) {
    auto data = reinterpret_cast<ThreadData*>(_data);

    // Set the name
    const char* threadName;
    if (!data->instance->name.Empty()) {
        threadName = data->instance->name.AsCharArray();
    } else {
        auto rtti = data->function.GetRTTI();
        threadName = rtti.name;
    }

    ThreadAPI::SetName(data->instance->handle, threadName);

    // Perform user-action
    // TODO: To encapsulate or not?
    //       Perhaps encapsulate on release and not debug to allow for bug catching and safety on release?
    //try {
    if (data->stack.IsValid()) {
        data->function.InvokeStack(data->stack);
    } else {
        data->function.Invoke();
    }
    /*} catch( ... ) {
        DJINN_ASSERT("User thread crashed");
    }*/

    // Set state
    LockGuard<Mutex> Guard(data->instance->dataMutex);
    data->instance->state = ThreadState::eIdle;

    // ...
    delete data;
}

void Thread::Start(const Core::DynamicDelegate& function, const Core::DynamicParameterStack& stack) {
    LockGuard<Mutex> Guard(dataMutex);

    // Create data
    ThreadData *data = new ThreadData();
    data->instance = this;
    data->function = function;
    data->stack = stack;

    // Set state
    state = ThreadState::eRunning;

    // Attempt to create thread
    handle = ThreadAPI::Start(Callback, data);

    // Set name
    if (name.Length() > 0) {
        ThreadAPI::SetName(handle, name.AsCharArray());
    }
}

ThreadState Thread::GetState() {
    return state;
}

void Thread::SetName(const Core::String &name) {
    this->name = name;
}
