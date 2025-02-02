//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Common.h>
#include <Core/Atomic.h>
#include <exception>

#ifdef DJINN_PLATFORM_WINDOWS
extern "C"
{
typedef struct _RTL_CRITICAL_SECTION RTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION* PRTL_CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;
DJINN_IMPORT void __stdcall InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
DJINN_IMPORT void __stdcall DeleteCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
DJINN_IMPORT void __stdcall EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
DJINN_IMPORT void __stdcall LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
DJINN_IMPORT int __stdcall TryEnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
}
#else
#include <pthread.h>
#endif

namespace Djinn {
    namespace Platform {
        // Standard recursive mutex using system calls
        class RecursiveMutex {
        public:
            DJINN_NOCOPY(RecursiveMutex);

            RecursiveMutex( );
            ~RecursiveMutex( );

            // Lock the mutex
            // - Will block until a lock is created
            DJINN_INLINE void Lock( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                EnterCriticalSection( handle );
#else
                pthread_mutex_lock(&handle);
#endif
            }

            // Lock the mutex
            // - Will block until a lock is created
            DJINN_INLINE void LockNotify( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                EnterCriticalSection( handle );
#else
                pthread_mutex_lock(&handle);
#endif
            }

            // Lock the mutex
            // - Will return if failed
            DJINN_INLINE bool TryLock( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                if (TryEnterCriticalSection( handle ) == 0) {
                    return false;
                }
                return true;
#else
                if (pthread_mutex_trylock(&handle) == 0) {
                    return false;
                }
                return true;
#endif
            }

            // Unlock the mutex
            DJINN_INLINE void Unlock( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                LeaveCriticalSection( handle );
#else
                pthread_mutex_unlock(&handle);
#endif
            }

            // Handle
#ifdef DJINN_PLATFORM_WINDOWS
            CRITICAL_SECTION* GetHandle() {
                return handle;
            }
#endif

            // Handle
#ifdef DJINN_PLATFORM_LINUX
            pthread_mutex_t* GetHandle() {
                return &handle;
            }
#endif

        private:
#ifdef DJINN_PLATFORM_WINDOWS
            CRITICAL_SECTION* handle = nullptr;
#else
            pthread_mutex_t handle;
#endif
        };
    }
}