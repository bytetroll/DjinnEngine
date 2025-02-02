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
 on 3/12/2018.
//

#include <Platform/ThreadAPI.h>
#include <Core/Common.h>
#include <sched.h>
#include <pthread.h>

DJINN_NS3(Platform, ThreadAPI);

namespace {
    struct ThreadData {
        void *userData;
        void (*callback)(void *);
    };
}

Handle ThreadAPI::Start(void(*callback)(void *), void *userData) {
    Handle handle;

    // Base thread data
    auto threadData = new ThreadData();
    threadData->userData = userData;
    threadData->callback = callback;

    // Start thread
    pthread_create(&handle.ptr, NULL, [](void *_data) -> void * {
        auto data = reinterpret_cast<ThreadData *>(_data);
        data->callback(data->userData);
        delete data;
        return nullptr;
    }, threadData);

    // ...
    return handle;
}

void ThreadAPI::SetName(const Handle& handle, const char *name) {
    pthread_setname_np(handle.ptr, name);
}

void ThreadAPI::YieldAvailable() {
    sched_yield();
}

ThreadID ThreadAPI::GetHandleID(const Handle &handle) {
    return reinterpret_cast<ThreadID >(handle.ptr);
}

void ThreadAPI::Detach(const Handle &handle) {
    pthread_detach(handle.ptr);
}

void ThreadAPI::Join(const Handle &handle) {
    pthread_join(handle.ptr, nullptr);
}