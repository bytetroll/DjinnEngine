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
 on 3/1/2018.
//

#include <Game/WorldHostSystem.h>
#include <Game/IWorld.h>
#include <Game/IWorldHost.h>
#include <Base/DefaultLogs.h>
#include <Base/IPipeline.h>

DJINN_NS2(Game);

WorldHostSystem::WorldHostSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : Game::IStepSystem(registry, outer, host) {
    // ...
}

COM::Result WorldHostSystem::Initialize(IWorldHost *host) {
    this->host = host;

    // Diagnostic
    InfoLog(this).Write("Initialized");
    return COM::kOK;
}

COM::Result WorldHostSystem::Run(StepMode mode, double delta) {
    Async::Group group;

    // Run the worlds through the execution pipeline
    // Will respect any inter-world dependencies
    host->GetPipeline()->RunAsyncAs<Game::IStepSystem>(group, [&](Game::IStepSystem* system) {
        Sink(system->Run(mode, delta));
    });

    // Wait
    group.Wait();
    return COM::kOK;
}

StepModeSet WorldHostSystem::GetStepMode() {
    return StepMode::eAll;
}

COM::Result WorldHostSystem::Configure(Base::PipelineBuilder& builder) {
    return COM::kOK;
}
