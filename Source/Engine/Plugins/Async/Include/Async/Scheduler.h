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

#include <Core/Array.h>
#include <Platform/ConditionalVar.h>
#include <Core/SparseArray.h>
#include "IScheduler.h"


namespace Djinn::Async {
    class Worker;

    class DJINN_EXPORT_PLUGINS_ASYNC Scheduler : public IScheduler {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Scheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this scheduler
        /// \return
        COM::Result Initialize();

        /// Get least load worker optimal for submission
        /// \return
        Worker *GetLeastLoadWorker(const LaneID& lane);

        /// Overrides
        virtual COM::Result CreateLane(const LaneID &lane, UInt32 workerCount) override;
        virtual void Schedule(UInt32 count, const ScheduledJob* jobs, const LaneID& lane) override;
        virtual void Schedule(UInt32 count, const YieldedJob* jobs) override;
        virtual void Yield() override;
        virtual void Yield(JobGroup* group) override;
        virtual void SetCallGraph(CallGraph *graph) override;
        virtual void SetOwner(void *owner) override;
        virtual UInt32 GetWorkerCount() override;

    private:
        Core::SparseArray<Core::Array<Worker *>> lanes;
        Platform::Mutex waitMtx;
        UInt32 insertionIndex = 0;
        CallGraph *callGraph = nullptr;
    };
}