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
#include <Platform/LockGuard.h>
#include <Async/ContextData.h>
#include <Core/Pool.h>
#include "Info.h"
#include "Lane.h"

namespace Djinn::Async {
    class IScheduler;
    class CallGraph;
    struct ScheduledJob;
    struct JobGroup;
    struct YieldedJob;

    class DJINN_EXPORT_PLUGINS_ASYNC Framework : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Framework(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this framework
        /// \param scheduler
        /// \return
        COM::Result Initialize(IScheduler *scheduler);

        /// Schedule
        /// \param jobs
        void Schedule(UInt32 count, const ScheduledJob *jobs, const LaneID &lane);

        /// Schedule
        /// \param jobs
        void Schedule(UInt32 count, const YieldedJob *jobs);

        /// Yield current job
        void Yield();

        /// Yield current job
        void Yield(JobGroup *group);

        /// Set the call graph
        /// \param callGraph 
        void SetCallGraph(CallGraph *callGraph);

        /// Get scheduler
        /// \return
        IScheduler *GetScheduler() const {
            return scheduler;
        }

    private:
        IScheduler *scheduler;
        ContextData defaultData;
    };
}
