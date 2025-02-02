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
 on 2/12/2018.
//
#pragma once

#include <Core/Atomic.h>
#include "Thread.h"
#include "ConcurrentQueue.h"
#include "ConditionalVar.h"

namespace Djinn::Platform {
    class Event;

    class SynchronizedWorkGroup {
    public:
        SynchronizedWorkGroup();
        ~SynchronizedWorkGroup();

        /// Queue a job for synchronized execution
        /// \param delegate
        /// \param event - If not null, signaled when delegate has finished invoking
        void Queue(const Core::Delegate<void()>& delegate, Event* event = nullptr);

    private:
        void __Thread();

        /// Thread
        Thread thread;
        ConditionalVar var;
        Mutex mutex;
        UInt32 jobCount = 0;

        /// Exit flag for thread exit
        Core::Atomic<bool> exitFlag;

        /// Job Container
        struct Job {
            Core::Delegate<void()> delegate;
            Event* event;
        };
        ConcurrentQueue<Job> workQueue;
    };
}