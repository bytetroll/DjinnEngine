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
 on 6/6/2017.
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
