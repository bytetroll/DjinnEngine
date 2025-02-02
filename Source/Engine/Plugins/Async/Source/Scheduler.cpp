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

#include <Async/Scheduler.h>
#include <Async/Worker.h>
#include <Async/CallGraph.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Async);

Scheduler::Scheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IScheduler(registry, outer, host) {

}

COM::Result Scheduler::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result Scheduler::CreateLane(const LaneID &lane, UInt32 workerCount) {
    // Spawn workers
    for (UInt32 i = 0; i < workerCount; i++) {
        // Create worker
        Worker *worker;
        if (!registry->CreateClass(this, &worker) || !worker->Initialize()) {
            return COM::kError;
        }

        // ...
        lanes[lane.Get()].Add(worker);
    }

    // Diagnostic
    InfoLog(this).Write("Created lane [", lane.Get(), "] with ", workerCount, " workers");

    // OK
    return COM::kOK;
}

Worker *Scheduler::GetLeastLoadWorker(const LaneID &lane) {
    USize load = static_cast<USize>(1e7);
    Worker *worker = nullptr;

    auto &workers = lanes[lane.Get()];
    for (USize i = 0; i < workers.Size(); i++) {
        USize _load = workers[i]->GetJobCount();
        if (_load < load) {
            worker = workers[i];
            load = _load;
        }
    }

    return worker;
}

void Scheduler::Schedule(UInt32 count, const ScheduledJob *jobs, const LaneID &lane) {
    if (count == 0) {
        return;
    }

    if (count == 1) {
        Worker *worker = GetLeastLoadWorker(lane);
        if (!worker) {
            return;
        }

        worker->Schedule(1, jobs);
    } else {
        auto &workers = lanes[lane.Get()];

        UInt32 offset = 0;
        UInt32 actionCount = static_cast<UInt32>(Math::Floor(count / workers.Size()));

        for (USize i = 0; i < workers.Size(); i++) {
            Worker *worker = GetLeastLoadWorker(lane);
            if (!worker) {
                return;
            }

            if (i == workers.Size() - 1) {
                worker->Schedule(count - offset, jobs + offset);
            } else if (actionCount != 0) {
                worker->Schedule(actionCount, jobs + offset);

                offset += actionCount;
            }
        }
    }
}

void Scheduler::Schedule(UInt32 count, const YieldedJob *jobs) {
    for (USize i = 0; i < count; i++) {
        reinterpret_cast<Worker *>(jobs[i].worker)->Schedule(jobs[i]);
    }
}

void Scheduler::Yield() {
    Worker::Yield();
}

void Scheduler::Yield(JobGroup *group) {
    Worker::Yield(group);
}

void Scheduler::SetOwner(void *owner) {
    for (auto it = lanes.It(); it; ++it) {
        for (auto &&worker : *it) {
            worker->SetOwner(owner);
        }
    }
}

UInt32 Scheduler::GetWorkerCount() {
    UInt32 count = 0;
    for (auto it = lanes.It(); it; ++it) {
        count += it->Size();
    }
    return count;
}

void Scheduler::SetCallGraph(CallGraph *graph) {
    if (callGraph) {
        callGraph->SetRecording(false);
    }

    if (graph) {
        graph->SetRecording(true);
    }

    callGraph = graph;
    for (auto it = lanes.It(); it; ++it) {
        for (auto &&worker : *it) {
            worker->SetCallGraph(callGraph);
        }
    }
}
