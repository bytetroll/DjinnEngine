//
// Created by Dev on 2/12/2018.
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