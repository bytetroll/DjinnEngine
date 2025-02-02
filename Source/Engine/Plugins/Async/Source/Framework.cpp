//
// Created by Dev on 1/4/2018.
//

#include <Async/Framework.h>
#include <Async/IScheduler.h>
#include <Async/ContextAPI.h>

DJINN_NS2(Async);

Framework::Framework(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}


COM::Result Framework::Initialize(IScheduler *scheduler) {
    this->scheduler = scheduler;

    // Set owner
    scheduler->SetOwner(this);

    // Default context data
    defaultData.owner = this;
    API::SetDefaultContextData(&defaultData);

    // OK
    return COM::kOK;
}

void Framework::Schedule(UInt32 count, const ScheduledJob *jobs, const LaneID &lane) {
    scheduler->Schedule(count, jobs, lane);
}

void Framework::Schedule(UInt32 count, const YieldedJob *jobs) {
    scheduler->Schedule(count, jobs);
}

void Framework::Yield() {
    scheduler->Yield();
}

void Framework::Yield(JobGroup *group) {
    scheduler->Yield(group);
}

void Framework::SetCallGraph(CallGraph *callGraph) {
    scheduler->SetCallGraph(callGraph);
}
