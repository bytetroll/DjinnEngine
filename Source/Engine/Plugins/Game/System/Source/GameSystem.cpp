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
 on 1/10/2018.
//

#include <Game/GameSystem.h>
#include <Game/IStepSystem.h>
#include <Base/DefaultLogs.h>
#include <chrono>
#include <thread>
#include <Async/CallGraph.h>
#include <Base/MemoryStatistics.h>

DJINN_NS2(Game);

GameSystem::GameSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IGameSystem(registry, outer, host) {

}

COM::Result GameSystem::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    // Create callgraphs
    asyncCallgraphPrimary = new Async::CallGraph();
    asyncCallgraphSecondary = new Async::CallGraph();

    // OK
    return COM::kOK;
}

COM::Result GameSystem::Run() {
    while (!exitFlag) {
        // Get capped delta
        delta = Math::Min(0.25, timer.Delta());

        // Recording?
        if (isAsyncRecording) {
            asyncCallgraphPrimary->Flush();
            Async::SetCallGraph(asyncCallgraphPrimary);
        } else {
            Async::SetCallGraph(nullptr);
        }

        // ...
        double maxDist = (1.0 / static_cast<double>(maxIPS)) - delta.Load();
        if (maxDist > 0) {
            // TODO: Custom time library, consider: Platform::API::ThreadXYZW...
            auto time = std::chrono::duration<double, std::milli>(maxDist * 1e3);
            //std::this_thread::sleep_for(time);
        }

        // ...
        accumulator += delta.Load();

        // Group
        Async::Group masterGroup;

        // Run remaining systems
        for (IStepSystem* system : systems) {
            if (system->GetStepMode() & StepMode::eAsync) {
                system->RunAsync(masterGroup, StepMode::eAsync, delta.Load());
            }
        }

        // Integrate
        while (accumulator >= fixedTimeStep) {
            // Group
            Async::Group group;

            // Run all integration systems
            for (IStepSystem* system : systems) {
                if (system->GetStepMode() & StepMode::eIntegral) {
                    system->RunAsync(group, StepMode::eIntegral, fixedTimeStep);
                }
            }

            // ...
            elapsedTime += fixedTimeStep;
            accumulator -= fixedTimeStep;

            // Wait
            group.Wait();
        }

        // Run remaining systems
        for (IStepSystem* system : systems) {
            if (system->GetStepMode() & StepMode::eOnce) {
                system->RunAsync(masterGroup, StepMode::eOnce, delta.Load());
            }
        }

        // Wait
        masterGroup.Wait();

        // Recording?
        if (isAsyncRecording) {
            // Swap
            auto tmp = asyncCallgraphPrimary;
            asyncCallgraphPrimary = asyncCallgraphSecondary;
            asyncCallgraphSecondary = tmp;
        }

        // ...
        iterationCounter++;
    }

    // Dont record outside of this system
    Async::SetCallGraph(nullptr);

    // OK
    return COM::kOK;
}


COM::Result GameSystem::Register(IStepSystem *system) {
    if (systems.Contains(system)) {
        return Base::kSystemDuplicate;
    }
    systems.Add(system);

    // OK
    return COM::kOK;
}

COM::Result GameSystem::Deregister(IStepSystem *system) {
    return systems.RemoveValue(system) ? COM::kOK : Base::kSystemNotFound;
}

void GameSystem::SetFixedStep(double step) {
    fixedTimeStep = step;
}

void GameSystem::FlagExit() {
    exitFlag = true;
}

void GameSystem::SetMaxIPS(UInt32 ips) {
    maxIPS = ips;
}

double GameSystem::GetDelta() {
    return delta.Load();
}

UInt32 GameSystem::GetMaxIPS() {
    return maxIPS;
}

void GameSystem::EnableAsyncRecording(bool b) {
    isAsyncRecording = b;
}

Async::CallGraph *GameSystem::GetAsyncGraph() {
    return asyncCallgraphSecondary;
}

COM::Result GameSystem::Configure(Base::PipelineBuilder& builder) {
    return COM::kOK;
}
