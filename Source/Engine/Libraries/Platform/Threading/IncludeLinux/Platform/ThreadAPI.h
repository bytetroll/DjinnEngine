#pragma once

#include <pthread.h>
//using pthread_t = struct pthread*;

namespace Djinn {
    namespace Platform {
        namespace ThreadAPI {
            using ThreadID = pthread_t;
            struct Handle {
                pthread_t ptr;
            };

            /// Current thread id
            ///extern thread_local ThreadID threadID;
            static ThreadID GetCurrentID() {
                return pthread_self();
            }

            /// Start a thread with given data
            /// \param callback
            /// \param userData
            /// \return
            extern Handle Start(void(*callback)(void*), void* userData);

            /// Get id from handle
            /// \param handle
            /// \return
            extern ThreadID GetHandleID(const Handle& handle);

            /// Join a handle
            /// \param handle
            extern void Join(const Handle& handle);

            /// Detach a handle
            /// \param handle
            extern void Detach(const Handle& handle);

            /// Set the name of this thread
            /// \param name
            extern void SetName(const Handle& handle, const char* name);

            /// Yield to any available thread to not block queues
            extern void YieldAvailable();
        }
    }
}
