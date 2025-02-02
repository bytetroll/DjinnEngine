//
// Created by Dev on 3/1/2018.
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
