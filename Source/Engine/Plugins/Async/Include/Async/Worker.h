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
 on 1/4/2018.
//
#pragma once

#include <Core/DynamicDelegate.h>
#include <Async/ContextAPI.h>
#include <COM/IUnknown.h>
#include <Core/Array.h>
#include <Core/SlotArray.h>
#include <Platform/ConditionalVar.h>
#include <Core/Pool.h>
#include <Platform/Thread.h>
#include <Platform/ConcurrentQueue.h>
#include <Core/Timer.h>
#include "Info.h"
#include "ContextData.h"
#include "CallGraph.h"
#include "ScheduledJob.h"


namespace Djinn::Async {
    class DJINN_EXPORT_PLUGINS_ASYNC Worker : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Worker(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this scheduler
        /// \return
        COM::Result Initialize();

        /// Schedule a job
        /// \param info
        void Schedule(UInt32 count, const ScheduledJob *jobs);

        /// Schedule a job
        /// \param info
        void Schedule(const YieldedJob &job);

        /// Set owner
        /// \param owner
        void SetOwner(void *owner);

        /// Set call graph
        /// \param graph
        void SetCallGraph(CallGraph *graph);

        /// Flag this scheduler to stop the worker thread, does not join
        void FlagExit() {
            exitFlag = true;
        }

        /// Get current job count
        /// \return
        USize GetJobCount() {
            return static_cast<USize>(jobCounter.Load());
        }

        /// Yield helper
        static void Yield();

        /// Yield helper
        static void Yield(JobGroup *group);

    private:
        /// Contained job state
        struct JobState {
            /// Current job
            ScheduledJob job;

            /// Backend context
            API::Context context = nullptr;

            /// Call graph indices
            CallGraph* graph = nullptr;
            CallHeader yieldCallIndex;
            CallHeader parentCallIndex;

            /// Current state
            bool isYielded = false;
        };

        /// Context data
        ContextData contextData;

        /// Running context data
        struct RunningContextData {
            JobState *state;
            API::Context context;
        } runningContextData;

        /// Thread context entry point
        /// \param threadContext
        void __ThreadContext();

        /// Context entry point
        void __Context();

        /// Track job count
        /// \param amount
        void Track(Int64 amount);

        /// Thread context
        API::Context threadContext;

    private:
        bool PopScheduledJob(ScheduledJob &out);
        bool PopYieldedJob(YieldedJob &out);

        /// Jobs
        Platform::Mutex jobMutex;
        Core::SlotArray<ScheduledJob> scheduledJobs;
        Core::SlotArray<YieldedJob> yieldedJobs;

        /// Total job count
        Platform::SpinMutex jobCounterMutex;
        Core::AtomicCounter jobCounter{0};

        /// Conditional wake var
        Platform::Mutex condMtx;
        Platform::ConditionalVar wakeVar;

        /// Internally owned contexts
        Core::ObjectPool<std::remove_pointer_t<API::Context>> contexts;

        /// Internally owned states
        Core::ObjectPool<JobState> states;

        /// Initial thread
        Platform::Thread thread;

        /// Exit flag
        Core::Atomic<bool> exitFlag{false};

        /// Call timer
        Core::Timer callTimer;

        /// Call graph
        Core::Atomic<CallGraph *> callGraph{nullptr};
    };
}