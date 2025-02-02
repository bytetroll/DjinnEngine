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
 on 2/28/2018.
//

#include <Game/World.h>
#include <Game/IWorldSystem.h>
#include <Base/ISystem.h>
#include <Host/IRegistry.h>
#include <Host/Pipeline.h>
#include <Base/DefaultLogs.h>
#include <Host/Pipeline.h>
#include <Base/Pipeline.h>
#include <Game/IScene.h>
#include <Game/SpatialScene.h>
#include <Game/IWorldPackage.h>

DJINN_NS2(Game);

World::World(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorld(registry, outer, host) {

}

COM::Result World::Initialize(Data::WorldConfig *config) {
    this->config = config;

    // Create pipeline
    pipeline = new Host::Pipeline();

    // Create system pipeline
    if (!registry->CreateClass(this, &systemPipeline)) {
        ErrorLog(this).Write("Failed to create system pipeline");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

Data::WorldConfig *World::GetConfig() {
    return config;
}

COM::Result World::Register(IWorldSystem *system) {
    return systemPipeline->Register(system);
}

COM::Result World::Deregister(IWorldSystem *system) {
    return systemPipeline->Deregister(system);
}

Host::IPipeline *World::GetPipeline() {
    return pipeline;
}

COM::Result World::Run(StepMode mode, double delta) {
    // Run all systems
    {
        Async::Group group;
        systemPipeline->RunAsyncAs<IWorldSystem>(group, [&](IWorldSystem *system) {
            if (system->GetStepMode() & mode) {
                Sink(system->Run(mode, delta));
            }
        });
    }

    // Build all scenes
    {
        Async::Group group;
        for (auto&& scene : scenes) {
            scene->BuildAsync(group);
        }
    }

    // OK
    return COM::kOK;
}

Base::IPipeline *World::GetSystemPipeline() {
    return systemPipeline;
}

Core::ArrayView<IScene *> World::GetScenes() {
    return scenes;
}

COM::Result World::CreateScene(COM::InterfaceID iid, ISceneState* state, IScene **out) {
    COM::ClassID *cid = sceneTypes.GetNullable(iid);
    if (!cid) {
        ErrorLog(this).Write("Attempted to create scene of unknown interface");
        return COM::kError;
    }

    // Create scene
    IScene* scene;
    CheckMsg(registry->CreateClass(*cid, this, &scene), Error, "Failed to create scene");

    // Initialize
    if (!scene->Initialize(this, state)) {
        return COM::kError;
    }

    // Install packages
    for (auto package : packages) {
        if (!package->Initialize(scene)) {
            ErrorLog(this).Write("Failed to initialize package [scene] '", package->GetClassName(), "'");
            return COM::kError;
        }
    }

    // ...
    *out = scene;

    // ...
    scenes.Add(scene);
    return COM::kOK;
}

void World::Register(IWorldPackage *package) {
    packages.Add(package);
}

void World::RegisterSceneType(COM::InterfaceID iid, COM::ClassID cid) {
    sceneTypes[iid] = cid;
}

COM::Result World::Configure(Base::PipelineBuilder &builder) {
    return COM::kOK;
}

StepModeSet World::GetStepMode() {
    return StepMode::eAll;
}
