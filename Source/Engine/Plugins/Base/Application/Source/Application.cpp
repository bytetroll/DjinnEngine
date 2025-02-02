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
 on 8/17/2017.
//

#include <Core/Common.h>
#include <Base/ISystem.h>
#include <Async/Async.h>
#include <Async/Scheduler.h>
#include <Async/Framework.h>
#include <Platform/User.h>
#include <Base/DefaultLogs.h>
#include <Base/IApplicationSystem.h>
#include "Base/Application.h"

DJINN_NS2(Base);

#define SYNC_CALLBACK

COM::Result Base::Launch(const Base::ApplicationInfo &info, const Core::Delegate<void(Base::Application &)> &callback) {
    Host::Registry registry;

    Base::Application* app = new Base::Application(&registry, nullptr, nullptr);

    if (COM::CheckedResult result = app->Initialize(info)) {
        Sink(app->Release());
        return result;
    }

#ifdef SYNC_CALLBACK
    if (COM::CheckedResult result = app->Run(true, [&, callback]() mutable {
        callback(*app);
    })) {
        return result;
    }
#else
    if (COM::CheckedResult result = app->Run(true, [&, callback]() mutable {
        callback(*app);
    })) {
        return result;
    }
#endif

    return app->Release();
}

Application::Application(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {
    // Install this
    Sink(registry->GetPipeline()->AddClass(this));
}

COM::Result Application::Initialize(const ApplicationInfo &info) {
    this->info = info;

    // Hello world!
    std::cout << "Starting application '" << info.name.AsCharArray() << "'\n";

    // Load logging
    if (!registry->LoadPlugin("Plugins.Base.Logging", Host::LoadFlag::eAll)) {
        std::cout << "Application initialization failed, could not load logging plugin\n";
        return COM::kError;
    }

    // Load default plugins
    if (!registry->LoadPlugin("Plugins.Base.Memory", Host::LoadFlag::eAll) ||
        !registry->LoadPlugin("Plugins.Reflection", Host::LoadFlag::eAll) ||
        !registry->LoadPlugin("Plugins.Serialization", Host::LoadFlag::eAll) ||
        !registry->LoadPlugin("Plugins.Async", Host::LoadFlag::eAll) ||
        !registry->LoadPlugin("Plugins.Base.System", Host::LoadFlag::eAll)) {
        ErrorLog(this).Write("One or more plugin initializations failed");
        return COM::kError;
    }

    // Find classes
    if (!registry->GetPipeline()->GetClass(&asyncFramework)) {
        ErrorLog(this).Write("One or more pipeline classes could not be found");
        return COM::kError;
    }

    // Create pipeline
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to create pipeline");
        return COM::kError;
    }

    // Diagnostic
    InfoLog(this).Write("Initialized");

    // OK
    return COM::kOK;
}

COM::Result Application::Run(bool block) {
    return Run(block, [] {});
}

COM::Result Application::Run(bool block, const Core::Delegate<void()> &asyncLoad) {
    // Result
    COM::Result result = COM::kOK;

    // Describe schedule
    Async::ScheduledJob job;
    job.isParameterized = true;
    job.parameterizedInfo.delegate = Bindff(__Run);
    job.parameterizedInfo.stack = {&result, asyncLoad};

    // Schedule
    asyncFramework->Schedule(1, &job, Async::kDefault);

    // Block?
    if (block) {
        blockVar.Wait(blockMtx);
    }

    // ...
    return result;
}

void Application::__Run(COM::Result *result, Core::Delegate<void()> asyncLoad) {
    *result = COM::kOK;

    // Blocked load
    Async::ScheduleTask(asyncLoad).Wait();

    // Run all systems
    Async::Group group;
    pipeline->RunAsyncAs<IApplicationSystem>(group, [&](IApplicationSystem *system) { Sink(system->Run()); });

    // Wait for all systems
    group.Wait();

    // Signal
    blockVar.SignalAll();
}

COM::Result Application::Register(IApplicationSystem *system) {
    return pipeline->Register(system);
}

COM::Result Application::Deregister(IApplicationSystem *system) {
    return pipeline->Deregister(system);
}
