//
// Created by Dev on 1/4/2018.
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