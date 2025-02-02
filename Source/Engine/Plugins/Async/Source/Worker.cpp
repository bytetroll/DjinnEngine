//
// Created by Dev on 1/4/2018.
//

#include <Async/Worker.h>
#include <Platform/LockGuard.h>
#include <Core/Math.h>
#include <Core/Time.h>
#include <Platform/ThreadAPI.h>
#include <Async/CallGraph.h>
#include <Async/CallGraph.h>
#include <Async/JobGroup.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Async);

Worker::Worker(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result Worker::Initialize() {
    contextData.userData = this;

    // Begin thread
    thread.Start([&] {
        API::StartThreadContext([](API::ThreadContextData *data) {
            reinterpret_cast<Worker *>(data->user)->threadContext = data->context;
            reinterpret_cast<Worker *>(data->user)->__ThreadContext();
        }, this);
    });

    // OK
    return COM::kOK;
}

void Worker::Schedule(UInt32 count, const ScheduledJob *jobs) {
    {
        Platform::LockGuard guard(jobMutex);
        scheduledJobs.AddArray(count, jobs);
    }
    Track(count);
}

void Worker::Schedule(const YieldedJob &job) {
    {
        Platform::LockGuard guard(jobMutex);
        yieldedJobs.Add(job);
    }
    Track(1);
}

void Worker::Yield() {
    auto self = reinterpret_cast<Worker *>(reinterpret_cast<ContextData *>(API::GetContextData())->userData);

    // Mark as yielded
    self->runningContextData.state->isYielded = true;

    // Push job
    YieldedJob job;
    job.workerState = self->runningContextData.state;
    job.worker = self;
    self->Schedule(job);

    // Call graph
    if (self->runningContextData.state->graph) {
        Platform::LockGuard guard(*self->runningContextData.state->graph);

        // Call graph may have flushed, if the call does not exist create a new call
        if (self->runningContextData.state->graph->Contains(self->runningContextData.state->yieldCallIndex)) {
            auto& call = self->runningContextData.state->graph->Get(self->runningContextData.state->yieldCallIndex);
            call.runningTime = self->callTimer.Delta();
            call.endTimeStamp = Core::Time::GetTimeStamp();
        } else {
            RecordedCall call;
            call.startTimeStamp = Core::Time::GetTimeStamp();
            call.threadID = Platform::ThreadAPI::GetCurrentID();
            call.name = self->runningContextData.state->job.name;
            if (self->runningContextData.state->job.isParameterized) {
                call.rtti = self->runningContextData.state->job.parameterizedInfo.delegate.GetRTTI();
            } else {
                call.rtti = self->runningContextData.state->job.info.delegate.GetRTTI();
            }
            call.runningTime = self->callTimer.Delta();
            call.endTimeStamp = Core::Time::GetTimeStamp();
            self->runningContextData.state->graph->Add(call);
        }
    }

    // Switch back to worker
    self->runningContextData.state->yieldCallIndex = {};
    API::SwitchToContext(self->threadContext);
}

void Worker::Yield(JobGroup *group) {
    auto self = reinterpret_cast<Worker *>(reinterpret_cast<ContextData *>(API::GetContextData())->userData);

    // Mark as yielded
    self->runningContextData.state->isYielded = true;

    YieldedJob job;
    job.workerState = self->runningContextData.state;
    job.worker = self;
    group->Add(job);

    // Call graph
    if (self->runningContextData.state->graph) {
        Platform::LockGuard guard(*self->runningContextData.state->graph);

        // Call graph may have flushed, if the call does not exist create a new call
        if (self->runningContextData.state->graph->Contains(self->runningContextData.state->yieldCallIndex)) {
            auto& call = self->runningContextData.state->graph->Get(self->runningContextData.state->yieldCallIndex);
            call.runningTime = self->callTimer.Delta();
            call.endTimeStamp = Core::Time::GetTimeStamp();
        } else {
            RecordedCall call;
            call.startTimeStamp = Core::Time::GetTimeStamp();
            call.threadID = Platform::ThreadAPI::GetCurrentID();
            call.name = self->runningContextData.state->job.name;
            if (self->runningContextData.state->job.isParameterized) {
                call.rtti = self->runningContextData.state->job.parameterizedInfo.delegate.GetRTTI();
            } else {
                call.rtti = self->runningContextData.state->job.info.delegate.GetRTTI();
            }
            call.runningTime = self->callTimer.Delta();
            call.endTimeStamp = Core::Time::GetTimeStamp();
            self->runningContextData.state->graph->Add(call);
        }
    }

    // Switch back to worker
    self->runningContextData.state->yieldCallIndex = {};
    API::SwitchToContext(self->threadContext);
}

void Worker::__ThreadContext() {
    while (!exitFlag.Load()) {
        // Wait for a job
        wakeVar.Wait(condMtx, [&] { return jobCounter.Load() > 0; });

        USize count;
        {
            Platform::LockGuard guard(jobMutex);
            count = scheduledJobs.Size();
        }

        // Scheduled jobs take priority
        if (count > 0) {
            // Pop
            if (!(runningContextData.context = contexts.PopOrNull())) {
                // Create context
                runningContextData.context = API::CreateContext([](void *contextData) { return reinterpret_cast<Worker *>(reinterpret_cast<ContextData *>(contextData)->userData)->__Context(); }, &contextData);
            }

            // Start context
            API::SwitchToContext(runningContextData.context);
        }

            // Otherwise try yielded job
        else if (YieldedJob job; PopYieldedJob(job)) {
            runningContextData.state = reinterpret_cast<JobState *>(job.workerState);
            runningContextData.context = runningContextData.state->context;

            if (runningContextData.state->graph) {
                Platform::LockGuard guard(*runningContextData.state->graph);

                RecordedCall call;
                call.startTimeStamp = Core::Time::GetTimeStamp();
                call.parent = runningContextData.state->parentCallIndex;
                call.threadID = Platform::ThreadAPI::GetCurrentID();
                call.lastYield = runningContextData.state->yieldCallIndex;
                call.name = runningContextData.state->job.name;
                if (runningContextData.state->job.isParameterized) {
                    call.rtti = runningContextData.state->job.parameterizedInfo.delegate.GetRTTI();
                } else {
                    call.rtti = runningContextData.state->job.info.delegate.GetRTTI();
                }
                runningContextData.state->yieldCallIndex = runningContextData.state->graph->Add(call);
                callTimer.Delta();
            }

            // Resume running context
            API::SwitchToContext(runningContextData.context);
        }
    }
}

void Worker::__Context() {
    while (!exitFlag.Load()) {
        ScheduledJob job;
        while (PopScheduledJob(job)) {

            // Prepare state
            runningContextData.state = states.Pop();
            runningContextData.state->job = job;
            runningContextData.state->context = runningContextData.context;
            runningContextData.state->isYielded = false;

            // Call graph
            auto callGraph = this->callGraph.Load();
            if (callGraph) {
                auto parentCall = runningContextData.state->parentCallIndex;

                RecordedCall call;
                call.startTimeStamp = Core::Time::GetTimeStamp();
                call.threadID = Platform::ThreadAPI::GetCurrentID();
                call.name = job.name;
                call.parent = parentCall;
                if (job.isParameterized) {
                    call.rtti = job.parameterizedInfo.delegate.GetRTTI();
                } else {
                    call.rtti = job.info.delegate.GetRTTI();
                }

                Platform::LockGuard guard(*callGraph);

                auto callIndex = callGraph->Add(call);
                runningContextData.state->graph = callGraph;
                runningContextData.state->parentCallIndex = callIndex;
                runningContextData.state->yieldCallIndex = callIndex;

                callTimer.Delta();
            }

            // Invoke delegate
            if (!job.isParameterized) {
                job.info.delegate.Invoke();
            } else if (job.parameterizedInfo.stack.IsValid()) {
                job.parameterizedInfo.delegate.InvokeStack(job.parameterizedInfo.stack);
            } else {
                job.parameterizedInfo.delegate.Invoke();
            }

            // Call graph
            // Parent call index is the call index at this point
            if (runningContextData.state->graph) {
                Platform::LockGuard guard(*runningContextData.state->graph);

                if (runningContextData.state->isYielded) {
                    // Call graph may have flushed, if the yielder does not exist create a new call
                    if (runningContextData.state->graph->Contains(runningContextData.state->yieldCallIndex)) {
                        auto call = runningContextData.state->graph->Get(runningContextData.state->yieldCallIndex);
                        call.runningTime = callTimer.Delta();
                        if (runningContextData.state->graph->Contains(runningContextData.state->yieldCallIndex)) {
                            call.lastYield = runningContextData.state->yieldCallIndex;
                        }
                        call.endTimeStamp = Core::Time::GetTimeStamp();
                        runningContextData.state->graph->Add(call);
                    } else {
                        RecordedCall call;
                        call.startTimeStamp = Core::Time::GetTimeStamp();
                        call.threadID = Platform::ThreadAPI::GetCurrentID();
                        call.name = job.name;
                        if (job.isParameterized) {
                            call.rtti = job.parameterizedInfo.delegate.GetRTTI();
                        } else {
                            call.rtti = job.info.delegate.GetRTTI();
                        }
                        call.runningTime = callTimer.Delta();
                        if (runningContextData.state->graph->Contains(runningContextData.state->yieldCallIndex)) {
                            call.lastYield = runningContextData.state->yieldCallIndex;
                        }
                        call.endTimeStamp = Core::Time::GetTimeStamp();
                        runningContextData.state->graph->Add(call);
                    }
                } else {
                    // Call graph may have flushed, if the call does not exist create a new call
                    if (runningContextData.state->graph->Contains(runningContextData.state->yieldCallIndex)) {
                        auto& call = runningContextData.state->graph->Get(runningContextData.state->yieldCallIndex);
                        call.runningTime = callTimer.Delta();
                        call.endTimeStamp = Core::Time::GetTimeStamp();
                    } else {
                        RecordedCall call;
                        call.startTimeStamp = Core::Time::GetTimeStamp();
                        call.threadID = Platform::ThreadAPI::GetCurrentID();
                        call.name = job.name;
                        if (job.isParameterized) {
                            call.rtti = job.parameterizedInfo.delegate.GetRTTI();
                        } else {
                            call.rtti = job.info.delegate.GetRTTI();
                        }
                        call.runningTime = callTimer.Delta();
                        call.endTimeStamp = Core::Time::GetTimeStamp();
                        runningContextData.state->graph->Add(call);
                    }
                }
                runningContextData.state->parentCallIndex = {};
                runningContextData.state->yieldCallIndex = {};
                runningContextData.state->graph = nullptr;
            }

            // Push state
            states.Push(runningContextData.state);

            // Apply post operation
            if (job.group) {
                job.group->Apply(job.post);
            }
        }

        // Push context
        contexts.Push(runningContextData.context);

        // Switch to main
        API::SwitchToContext(threadContext);
    }
}


void Worker::SetOwner(void *owner) {
    contextData.owner = owner;
}

void Worker::Track(Int64 amount) {
    Platform::LockGuard guard(condMtx);
    if ((jobCounter += amount) > 0) {
        wakeVar.SignalAll();
    }
}

void Worker::SetCallGraph(CallGraph *graph) {
    this->callGraph = graph;
}

bool Worker::PopScheduledJob(ScheduledJob &out) {
    {
        Platform::LockGuard guard(jobMutex);
        if (scheduledJobs.Size() == 0) {
            return false;
        }
        out = scheduledJobs.First();
        scheduledJobs.Remove(0);
    }
    Track(-1);
    return true;
}

bool Worker::PopYieldedJob(YieldedJob &out) {
    {
        Platform::LockGuard guard(jobMutex);
        if (yieldedJobs.Size() == 0) {
            return false;
        }
        out = yieldedJobs.First();
        yieldedJobs.Remove(0);
    }
    Track(-1);
    return true;
}