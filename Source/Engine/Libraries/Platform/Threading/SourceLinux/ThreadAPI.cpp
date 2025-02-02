//
// Created by Dev on 3/12/2018.
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