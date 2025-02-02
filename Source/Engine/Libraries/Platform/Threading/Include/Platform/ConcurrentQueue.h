//
// Created by Dev on 9/14/2017.
//

#pragma once

// TODO: Is this the implementation that we should use?
// ©2013-2016 Cameron Desrochers.
// Distributed under the simplified BSD license (see the license file that
// should have come with this header).

#include <Core/Common.h>
#include <Platform/Detail/concurrentqueue.h>
#include <Core/Size.h>
#include <Core/Queue.h>

namespace Djinn::Platform {
    template <typename T>
    class ConcurrentQueue {
    public:
        // Clear queue
        DJINN_INLINE void Clear() {
            T tmp;
            while (handle.try_dequeue(tmp));
            //Platform::LockGuard guard(mutex);
            //queue.Clear(false);
        }

        // Add element
        DJINN_INLINE void Add(T &&value) {
            handle.enqueue(value);
            //Platform::LockGuard guard(mutex);
            //queue.Add(value);
        }

        // Add element
        DJINN_INLINE void Add(const T &value) {
            handle.enqueue(value);
            //Platform::LockGuard guard(mutex);
            //queue.Add(value);
        }

        // Pop element
        DJINN_INLINE bool Pop(T &out) {
            return handle.try_dequeue(out);
            //Platform::LockGuard guard(mutex);
            //return queue.Pop(out);
        }

        // Size
        DJINN_INLINE USize Size() {
            return handle.size_approx();
            //Platform::LockGuard guard(mutex);
            //return queue.Size();
        }

    private:
        // Backend handler
        moodycamel::ConcurrentQueue<T> handle;
        //Platform::Mutex mutex;
        //Core::Queue<T> queue;
    };
}