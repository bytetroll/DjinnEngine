//
// Created by Dev on 2/12/2018.
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
