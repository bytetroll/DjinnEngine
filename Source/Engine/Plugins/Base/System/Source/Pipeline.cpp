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
 on 5/2/2018.
//

#include <Base/Pipeline.h>
#include <Base/ISystem.h>
#include <Base/PipelineBuilder.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Base);

Pipeline::Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPipeline(registry, outer, host) {

}

void Pipeline::SetConfigurationMode(PipelineConfigurationMode mode) {
    this->mode = mode;
}

COM::Result Pipeline::Configure() {
    // Allow previous registrations to take effect
    container.ApplyChanges();
    container.ClearDependencies();

    // Add uniform dependencies
    for (auto&& dependency : uniformDependencies) {
        container.AddDependency(dependency.first, dependency.second);
    }

    // Configure all systems
    COM::Result result = COM::kOK;
    container.All([&](ISystem *system) {
        // Prepare the builder
        PipelineBuilder builder;

        // Attempt to configure
        if (COM::CheckedResult _result = system->Configure(builder)) {
            ErrorLog(this).Write("Failed to configure '", system->GetClassName(), "'");
            result = _result;
            return;
        }

        // Account for class dependencies
        for (auto&&[mode, cid] : builder.classDependencies) {
            ISystem *dependency;
            if (COM::CheckedResult _result = GetClass(cid, &dependency)) {
                ErrorLog(this).Write("Invalid class dependency in '", system->GetClassName(), "'");
                result = _result;
                return;
            }

            switch (mode) {
                case PipelineBuilderMode::eBefore: {
                    container.AddDependency(dependency, system);
                    break;
                }
                case PipelineBuilderMode::eAfter: {
                    container.AddDependency(system, dependency);
                    break;
                }
            }
        }

        // Account for interface dependencies
        for (auto&&[mode, iid] : builder.interfaceDependencies) {
            ISystem *dependency;
            if (COM::CheckedResult _result = GetInterface(iid, &dependency)) {
                ErrorLog(this).Write("Invalid interface dependency in '", system->GetClassName(), "'");
                result = _result;
                return;
            }

            switch (mode) {
                case PipelineBuilderMode::eBefore: {
                    container.AddDependency(dependency, system);
                    break;
                }
                case PipelineBuilderMode::eAfter: {
                    container.AddDependency(system, dependency);
                    break;
                }
            }
        }

        // Account for system dependencies
        for (auto&&[mode, dependency] : builder.systemDependencies) {
            switch (mode) {
                case PipelineBuilderMode::eBefore: {
                    container.AddDependency(dependency, system);
                    break;
                }
                case PipelineBuilderMode::eAfter: {
                    container.AddDependency(system, dependency);
                    break;
                }
            }
        }
    });

    // ...
    container.ApplyChanges();
    return result;
}

COM::Result Pipeline::Register(ISystem *system) {
    Platform::LockGuard guard(mutex);
    container.Add(system);
    systems.Add(system);
    classes[system->GetClassID()] = system;
    if (mode == PipelineConfigurationMode::eRegistration) {
        Check(Configure());
    } else {
        dirty = true;
    }
    return COM::kOK;
}

COM::Result Pipeline::Deregister(ISystem *system) {
    Platform::LockGuard guard(mutex);
    systems.RemoveValue(system);
    container.Remove(system);
    return COM::kOK;
}

COM::Result Pipeline::AddDependency(ISystem *source, ISystem *dest) {
    Platform::LockGuard guard(mutex);
    dirty = true;
    uniformDependencies.Emplace(source, dest);
    return COM::kOK;
}

COM::Result Pipeline::GetClass(const COM::ClassID &id, ISystem **out) {
    Platform::LockGuard guard(mutex);
    auto it = classes.Find(id);
    if (it == classes.End()) {
        return COM::kError;
    }

    *out = (*it).second;
    return COM::kOK;
}

COM::Result Pipeline::GetInterface(const COM::InterfaceID &id, ISystem **out) {
    Platform::LockGuard guard(mutex);
    for (auto system : systems) {
        void* dummy;
        if (system->QueryInterface(id, &dummy) && system->QueryInterface(out)) {
            return COM::kOK;
        }
    }
    return COM::kError;
}

void Pipeline::RunAsync(Async::Group &group, const Core::Delegate<void(ISystem *)> &delegate) {
    if (dirty && mode == PipelineConfigurationMode::eIteration) {
        CheckVoid(Configure());
        dirty = false;
    }
    container.Run(group, delegate);
}

COM::Result Pipeline::RegisterProxy(const COM::ClassID &cid, ISystem *system) {
    Platform::LockGuard guard(mutex);
    classes.Add(cid, system);
    return COM::kOK;
}

