//
// Created by Dev on 1/4/2018.
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
