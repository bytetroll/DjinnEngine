//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <Core/Common.h>
#include <Core/Types.h>
#include "Mutex.h"
#include <iostream>
#include <Core/Atomic.h>

#ifdef DJINN_PLATFORM_WINDOWS
#include <Core/Windows.h>
#include <errhandlingapi.h>
#else
#include <pthread.h>
#endif

namespace Djinn {
    namespace Platform {
        // Conditional variable using mutexes
        class ConditionalVar {
        public:
            ConditionalVar() {
#ifdef DJINN_PLATFORM_WINDOWS
                InitializeConditionVariable(&var);
#else
                pthread_cond_init(&var, nullptr);
#endif
            }

            ~ConditionalVar() {
#ifdef DJINN_PLATFORM_WINDOWS
#else
                pthread_cond_destroy(&var);
#endif
            }

            // Wait
            // - Will block until a the predicate is true
            template<typename MUTEX, typename FUNCTOR>
            DJINN_INLINE void Wait(MUTEX &mutex, FUNCTOR functor, unsigned long timeout = 0xffffffff) {
                ++count;
                mutex.LockNotify();
                while (!functor()) {
                    // May be cached, try again
                    if (--pending >= 0) {
                        if (functor()) {
                            break;
                        }
                    } else {
                        ++pending;
                    }

#ifdef DJINN_PLATFORM_WINDOWS
                    SleepConditionVariableCS(&var, mutex.GetHandle(), timeout);
#else
                    pthread_cond_wait(&var, mutex.GetHandle());
#endif
                }
                mutex.Unlock();
                --count;
            }

            // Wait
            // - Will block until a the predicate is true
            template<typename MUTEX, typename FUNCTOR>
            DJINN_INLINE void WaitNoUnlock(MUTEX &mutex, FUNCTOR functor, unsigned long timeout = 0xffffffff) {
                ++count;
                mutex.LockNotify();
                while (!functor()) {
                    // May be cached, try again
                    if (--pending >= 0) {
                        if (functor()) {
                            break;
                        }
                    } else {
                        ++pending;
                    }

#ifdef DJINN_PLATFORM_WINDOWS
                    SleepConditionVariableCS(&var, mutex.GetHandle(), timeout);
#else
                    pthread_cond_wait(&var, mutex.GetHandle());
#endif
                }
                --count;
            }

            // Wait
            // - Will block until a signal
            template<typename MUTEX>
            DJINN_INLINE void Wait(MUTEX &mutex, unsigned long timeout = 0xffffffff) {
                ++count;
#ifdef DJINN_PLATFORM_WINDOWS
                mutex.LockNotify();
                SleepConditionVariableCS(&var, mutex.GetHandle(), timeout);
                mutex.Unlock();
#else
                mutex.LockNotify();
                pthread_cond_wait(&var, mutex.GetHandle());
                mutex.Unlock();
#endif
                --count;
            }

            // Signal
            DJINN_INLINE void Signal() {
                ++pending;
#ifdef DJINN_PLATFORM_WINDOWS
                WakeConditionVariable(&var);
#else
                pthread_cond_signal(&var);
#endif
            }

            // Signal
            DJINN_INLINE void Signal(int n) {
#ifdef DJINN_PLATFORM_WINDOWS
                for (int i = 0; i < n; i++) {
                    ++pending;
                    WakeConditionVariable(&var);
                }
#else
                for (int i = 0; i < n; i++) {
                    ++pending;
                    pthread_cond_signal(&var);
                }
#endif
            }

            /// Signal all waiting threads
            /// \param threadCount - Total expected waiting threads, for pending wakes
            DJINN_INLINE void SignalAll(Int64 threadCount = 0) {
                pending += threadCount ?: count.Load();
#ifdef DJINN_PLATFORM_WINDOWS
                WakeAllConditionVariable(&var);
#else
                pthread_cond_broadcast(&var);
#endif
            }

            // Get number of waiting threads
            Int64 Count() {
                return static_cast<Int64>(count);
            }

            // Flush pending wakes
            void Flush() {
                pending = 0;
            }

        private:
#ifdef DJINN_PLATFORM_WINDOWS
            CONDITION_VARIABLE var = {};
#else
            pthread_cond_t var;
#endif

            Core::AtomicCounter count;

            // Pending number of wakes
            Core::AtomicCounter pending;
        };
    }
}