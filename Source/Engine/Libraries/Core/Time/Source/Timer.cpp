//
// Created by Dev on 6/6/2017.
//

#include <Core/Timer.h>
#include <Core/Common.h>

DJINN_NS2(Core);

Timer::Timer() {
#ifdef DJINN_PLATFORM_WINDOWS
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);
#endif
#ifdef DJINN_PLATFORM_LINUX
    clock_gettime(CLOCK_REALTIME, &t1);
#endif
}

double Timer::Delta() {
#ifdef DJINN_PLATFORM_WINDOWS
    // Record step
    QueryPerformanceCounter(&t2);

    // To seconds
    double Delta = static_cast<double>(t2.QuadPart - t1.QuadPart) / static_cast<double>(frequency.QuadPart);

    // ...
    t1 = t2;
    return Delta;
#endif
#ifdef DJINN_PLATFORM_LINUX
    // Record
    struct timespec t2;
    clock_gettime(CLOCK_REALTIME, &t2);

    // To seconds
    double Delta = (t2.tv_sec - t1.tv_sec);
    Delta += static_cast<double>(t2.tv_nsec - t1.tv_nsec) / 1e9;

    // ...
    t1 = t2;
    return Delta;
#endif
}

double Timer::DeltaRepeat() {
#ifdef DJINN_PLATFORM_WINDOWS
    // Record step
    QueryPerformanceCounter(&t2);

    // To seconds
    double Delta = static_cast<double>(t2.QuadPart - t1.QuadPart) / static_cast<double>(frequency.QuadPart);

    // ...
    return Delta;
#endif
#ifdef DJINN_PLATFORM_LINUX
    // Record
    struct timespec t2;
    clock_gettime(CLOCK_REALTIME, &t2);

    // To seconds
    double Delta = (t2.tv_sec - t1.tv_sec);
    Delta += static_cast<double>(t2.tv_nsec - t1.tv_nsec) / 1e9;

    // ...
    return Delta;
#endif
}
