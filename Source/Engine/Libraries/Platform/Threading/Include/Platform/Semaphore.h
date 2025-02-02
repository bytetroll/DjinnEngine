//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Common.h>

#ifdef DJINN_PLATFORM_WINDOWS
#include <synchapi.h>
#include <rpc.h>
#else
#include <pthread>
#endif

namespace Djinn {
    namespace Platform {
        class Semaphore {
        public:
            Semaphore( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                singleEvent = CreateEvent( 0, 0, 0, 0 );
                broadcastEvent = CreateEvent( 0, 1, 0, 0 );
#else
                pthread_cond_init(&Handle, nullptr);
#endif
            }

            ~Semaphore( ) {
#ifdef DJINN_PLATFORM_WINDOWS
#else
                pthread_cond_destroy(&Handle, nullptr);
#endif
            }

            // Wait for the condition
            template< class T >
            DJINN_INLINE void Wait( T* mutex ) {
#ifdef DJINN_PLATFORM_WINDOWS
                EnterCriticalSection( & waitersCountLock );
                ++waitersCount;
                LeaveCriticalSection( & waitersCountLock );
                mutex->Unlock( );
                {
                    // Listen to either
                    const HANDLE Events[] = { singleEvent, broadcastEvent };
                    DWORD R = WaitForMultipleObjects( 2, Events, 0, 0xffffffff );

                    // Check if last listener and of broadcast type
                    EnterCriticalSection( & waitersCountLock );
                    --waitersCount;
                    bool IsLast = ( waitersCount == 0 && R == ( WAIT_OBJECT_0 + 1 ) );
                    LeaveCriticalSection( & waitersCountLock );

                    // If last and broadcast type
                    if( IsLast )
                        ResetEvent( broadcastEvent );
                }
                mutex->Lock( );
#else
                pthread_cond_wait(&Handle, &Mutex->Handle);
#endif
            }

            // Notify a single thread that is currently waiting for this condition
            DJINN_INLINE void NotifySingle( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                EnterCriticalSection( & waitersCountLock );
                bool haveWaiters = ( waitersCount > 0 );
                LeaveCriticalSection( & waitersCountLock );
                if( haveWaiters )
                    SetEvent( singleEvent );
#else
                pthread_cond_signal(&Handle);
#endif
            }

            // Notify all threads that are currently waiting for this condition
            DJINN_INLINE void NotifyAll( ) {
#ifdef DJINN_PLATFORM_WINDOWS
                EnterCriticalSection( & waitersCountLock );
                bool haveWaiters = ( waitersCount > 0 );
                LeaveCriticalSection( & waitersCountLock );
                if( haveWaiters )
                    SetEvent( broadcastEvent );
#else
                pthread_cond_broadcast(&Handle);
#endif
            }

        private:
#ifdef DJINN_PLATFORM_WINDOWS
            HANDLE singleEvent;
            HANDLE broadcastEvent;
            unsigned int waitersCount;
            CRITICAL_SECTION waitersCountLock;
#else
            pthread_cond_t Handle;
#endif
        };
    }
}