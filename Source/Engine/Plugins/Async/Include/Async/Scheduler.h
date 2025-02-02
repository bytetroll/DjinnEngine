//
// Created by Dev on 1/4/2018.
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