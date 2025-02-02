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

#include <COM/IUnknown.h>
#include "Info.h"
#include "Lane.h"

namespace Djinn::Async {
    class CallGraph;
    struct JobGroup;
    struct ScheduledJob;
    struct YieldedJob;

    class IScheduler : public COM::TUnknown<IScheduler> {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IScheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Create a new lane
        /// \param lane
        /// \param workers
        virtual COM::Result CreateLane(const LaneID &lane, UInt32 workerCount) = 0;

        /// Schedule jobs
        /// \param info
        virtual void Schedule(UInt32 count, const ScheduledJob *jobs, const LaneID &lane) = 0;

        /// Schedule jobs
        /// \param info
        virtual void Schedule(UInt32 count, const YieldedJob *jobs) = 0;

        /// Yield from the current job
        virtual void Yield() = 0;

        /// Yield from the current job
        virtual void Yield(JobGroup *group) = 0;

        /// Set callgraph
        /// \param graph
        virtual void SetCallGraph(CallGraph *graph) = 0;

        /// Get worker count
        /// \return
        virtual UInt32 GetWorkerCount() = 0;

        /// Set context owner
        /// \param owner
        virtual void SetOwner(void *owner) = 0;
    };
}