//
// Created by Dev on 6/3/2018.
//

#include <Platform/RecursiveMutex.h>
#ifdef DJINN_PLATFORM_WINDOWS
#include <Core/Windows.h>
#else
#include <pthread.h>
#endif

DJINN_NS2(Platform);

RecursiveMutex::RecursiveMutex() {
#ifdef DJINN_PLATFORM_WINDOWS
    InitializeCriticalSection( handle = new CRITICAL_SECTION() );
#else
    pthread_mutex_init(&handle, NULL);
#endif
}

RecursiveMutex::~RecursiveMutex() {
#ifdef DJINN_PLATFORM_WINDOWS
    DeleteCriticalSection( handle );
#else
    pthread_mutex_destroy(&handle);
#endif
}
