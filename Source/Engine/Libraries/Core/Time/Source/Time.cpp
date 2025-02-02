//
// Created by Dev on 11/11/2018.
//

#include <Core/Time.h>
#include <Core/Common.h>

#ifdef DJINN_PLATFORM_WINDOWS
#include <Core/Windows.h>

namespace {
    LARGE_INTEGER GetFrequency() {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        return frequency;
    }
}
#else
#include <time.h>
#endif

DJINN_NS2(Core);

double Time::GetTimeStamp() {
#ifdef DJINN_PLATFORM_WINDOWS
    static LARGE_INTEGER frequency = GetFrequency();

    LARGE_INTEGER stamp;
    QueryPerformanceCounter(&stamp);

    return static_cast<double>(stamp.QuadPart) / frequency.QuadPart;
#else
    struct timespec stamp;
    clock_gettime(CLOCK_REALTIME, &stamp);

    return stamp.tv_sec + static_cast<double>(stamp.tv_nsec) / 1e9;
#endif
}