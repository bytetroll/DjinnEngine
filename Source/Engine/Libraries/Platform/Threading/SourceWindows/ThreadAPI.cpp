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
#include <Core/Windows.h>
#include <process.h>

DJINN_NS3(Platform, ThreadAPI);

namespace {
    typedef struct tagTHREADNAME_INFO {
        DWORD dwType; // Must be 0x1000.
        LPCSTR szName; // Pointer to name (in user addr space).
        DWORD dwThreadID; // Thread ID (-1=caller thread).
        DWORD dwFlags; // Reserved for future use, must be zero.
    } THREADNAME_INFO;

    const DWORD MS_VC_EXCEPTION = 0x406D1388;

    struct ThreadData {
        void *userData;
        void(*callback)(void *);
    };
}

static unsigned WINAPI ThreadStart(void *_data) {
    auto data = reinterpret_cast<ThreadData *>(_data);
    data->callback(data->userData);
    delete data;
    return 0;
}

Handle ThreadAPI::Start(void(*callback)(void *), void *userData) {
    Handle handle;

    // Base thread data
    auto threadData = new ThreadData();
    threadData->userData = userData;
    threadData->callback = callback;

    // Start thread
    handle.ptr = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, ThreadStart, threadData, 0, &handle.tid));

    // ...
    return handle;
}

void ThreadAPI::YieldAvailable() {
    Sleep(0);
}

ThreadID ThreadAPI::GetHandleID(const Handle &handle) {
    return handle.tid;
}

void ThreadAPI::SetName(const Handle& handle, const char *name) {
    THREADNAME_INFO info = {};
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = handle.tid;
    info.dwFlags = 0;
    __try {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        // TODO: Failed to set name
    }
}

void ThreadAPI::Detach(const Handle &handle) {
    CloseHandle(handle.ptr);
}

void ThreadAPI::Join(const Handle &handle) {
    WaitForSingleObject(handle.ptr, 0xffffffff);
    CloseHandle(handle.ptr);
}

