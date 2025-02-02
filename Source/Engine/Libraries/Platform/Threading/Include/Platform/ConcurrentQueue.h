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
 on 9/14/2017.
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