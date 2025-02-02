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

#include <Platform/SynchronizedWorkGroup.h>
#include <Platform/Event.h>

DJINN_NS2(Platform);

SynchronizedWorkGroup::SynchronizedWorkGroup() {
    thread.Start(Bindff(__Thread));
}

SynchronizedWorkGroup::~SynchronizedWorkGroup() {
    exitFlag = true;
    var.SignalAll();
    thread.Join();
}

void SynchronizedWorkGroup::Queue(const Core::Delegate<void()> &delegate, Event* event) {
    Job job;
    job.delegate = delegate;
    job.event = event;
    workQueue.Add(job);

    {
        LockGuard guard(mutex);
        jobCount++;
        var.Signal();
    }
}

void SynchronizedWorkGroup::__Thread() {
    while (!exitFlag.Load()) {
        var.Wait(mutex, [&] { return jobCount != 0 || exitFlag; });

        Job job;
        while (workQueue.Pop(job)) {
            job.delegate.Invoke();
            if (job.event) {
                job.event->Signal();
            }

            {
                LockGuard guard(mutex);
                jobCount--;
                var.Signal();
            }
        }
    }
}
