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
 on 1/17/2018.
//

#include <Game/EntitySystem.h>
#include <Base/ISystem.h>
#include <Base/DefaultLogs.h>
#include <Base/Pipeline.h>
#include <Game/IEntityComponent.h>
#include <Game/IEntityComponentHost.h>
#include <Graphics/IPacketSystem.h>

DJINN_NS2(Game);

EntitySystem::EntitySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IEntitySystem(registry, outer, host) {
    // ...
}

COM::Result EntitySystem::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");

    // Create system pipeline
    if (!registry->CreateClass(this, &pipeline)) {
        ErrorLog(this).Write("Failed to create pipeline");
        return COM::kError;
    }

    // ...
    return COM::kOK;
}

COM::Result EntitySystem::Queue(IEntityComponent *system, const DeferredEntityEvent &event) {
    auto &meta = componentMetas[system];
    Platform::LockGuard guard(meta.mutex);
    meta.events.Add(event);
    return COM::kOK;
}
COM::Result EntitySystem::Queue(const DeferredEntityEvent &event) {
    Platform::LockGuard guard(freeEventMutex);
    freeEvents.Add(event);
    return COM::kOK;
}

COM::Result EntitySystem::Run(StepMode mode, double delta) {
    // Update all systems
    {
        // Fire free events
        {
            Platform::LockGuard guard(freeEventMutex);
            stagedFreeEvents.Clear();
            stagedFreeEvents.Add(freeEvents);
            freeEvents.Clear();
        }
        for (auto&& event : stagedFreeEvents) {
            event.delegate.InvokeStack(event.stack);
        }

        Async::Group group;
        pipeline->RunAsyncAs<IEntityComponent>(group, [&](IEntityComponent *system) {
            auto &meta = componentMetas[system];

            // Fire events
            auto host = system->GetHost();
            {
                {
                    Platform::LockGuard guard(meta.mutex);
                    meta.stagedEvents.Clear();
                    meta.stagedEvents.Add(meta.events);
                    meta.events.Clear();
                }
                for (auto&& event : meta.stagedEvents) {
                    IEntityComponentState *dummy;
                    if (host->GetComponentPtr(event.id, &dummy)) {
                        event.delegate.InvokeStack(event.stack);
                    }
                }
            }

            // Update system
            system->Update(delta);
        });

        // Wait
        group.Wait();
    }

    {
        Platform::LockGuard guard(entityMutex);

        for (auto id : deletions) {
            USize header;
            if (!indices.Get(id.id, header)) {
                continue;
            }

            // Remove component
            if (!entities.Remove(header)) {
                return COM::kError;
            }

            // Update index array
            if (entities.Size() > header) {
                indices[entities[header].cid] = header;
            }
        }
        deletions.Clear();
    }

    // Post update
    for (auto csys : components) {
        Check(csys->GetHost()->ApplyChanges());

        csys->PostUpdate();
    }

    // Commit
    commitIndex++;

    // OK
    return COM::kOK;
}

COM::Result EntitySystem::Register(IEntityComponent *system) {
    components.Add(system);
    componentMap[system->GetClassID()] = system;
    componentMetas[system] = {};

    if (!pipeline->Register(system)) {
        return COM::kError;
    }

    return COM::kOK;
}

COM::Result EntitySystem::Deregister(IEntityComponent *system) {
    components.RemoveValue(system);
    componentMap.Remove(system->GetClassID());
    return pipeline->Deregister(system);
}

COM::Result EntitySystem::GetComponentSystem(const COM::ClassID &ccid, IEntityComponent **out) {
    auto it = componentMap.Find(ccid);
    if (it == componentMap.End()) {
        return COM::kError;
    }

    *out = (*it).second;
    return COM::kOK;
}

StepModeSet EntitySystem::GetStepMode() {
    return StepMode::eIntegral;
}

Int64 *EntitySystem::GetCommitHandle() {
    return &commitIndex;
}

Base::IPipeline *EntitySystem::GetPipeline() {
    return pipeline;
}

COM::Result EntitySystem::Configure(Base::PipelineBuilder& builder) {
    builder.Before<Graphics::IPacketSystem>();
    return COM::kOK;
}

COM::Result EntitySystem::CreateEntity(EntityReference *out) {
    Platform::LockGuard guard(entityMutex);

    // Create a component
    EntityMeta meta;
    meta.cid = entityCounter++;
    entities.Add(meta);

    // Add index
    indices[meta.cid] = entities.Size() - 1;

    // Set id
    out->id = {meta.cid};
    out->system = this;

    // OK
    return COM::kOK;
}

COM::Result EntitySystem::DestroyEntity(const EntityReference &ref) {
    Platform::LockGuard guard(entityMutex);

    // TODO: Quick fix
    for (auto csys : components) {
        auto host = csys->GetHost();

        auto opaque = OpaqueEntityComponent(EntityComponentReference(ref, csys, host), GetCommitHandle(), csys->GetClassID());
        if (opaque) {
            csys->Destroyed(Entity(ref), opaque);
        }

        // TODO: Hack
        Sink(csys->GetHost()->DestroyComponent(ref.id));
    }

    // Get header index
    USize header;
    if (!indices.Get(ref.id.id, header)) {
        return COM::kError;
    }

    // Remove index
    indices.Remove(ref.id.id);

    // Ignore duplicate deletions
    if (entities[header].deleted) {
        return COM::kOK;
    }

    // Mark as deleted
    entities[header].deleted = true;

    // Deferred deletion
    deletions.Add(ref.id);

    // OK
    return COM::kOK;
}

COM::Result EntitySystem::CreateComponent(const EntityReference &entity, COM::ClassID ccid, EntityComponentReference *out) {
    out->entity = entity;

    out->component = componentMap[ccid];
    if (!out->component) {
        return COM::kError;
    }

    out->host = out->component->GetHost();

    if (!out->host->CreateComponent(*out)) {
        return COM::kError;
    }

    out->component->Created(Entity(entity), OpaqueEntityComponent(*out, GetCommitHandle(), ccid));

    return COM::kOK;
}

COM::Result EntitySystem::DestroyComponent(const EntityComponentReference &ref) {
    ref.component->Destroyed(Entity(ref.entity), OpaqueEntityComponent(ref, GetCommitHandle(), ref.component->GetClassID()));

    if (!ref.host->DestroyComponent(ref.entity.id)) {
        return COM::kError;
    }

    return COM::kOK;
}

COM::Result EntitySystem::EnableComponent(const EntityComponentReference &ref) {
    if (!ref.host->Enable(ref.entity.id)) {
        return COM::kError;
    }

    ref.component->Enabled(Entity(ref.entity), OpaqueEntityComponent(ref, GetCommitHandle(), ref.component->GetClassID()));

    return COM::kOK;
}

COM::Result EntitySystem::DisableComponent(const EntityComponentReference &ref) {
    if (!ref.host->Disable(ref.entity.id)) {
        return COM::kError;
    }

    ref.component->Disabled(Entity(ref.entity), OpaqueEntityComponent(ref, GetCommitHandle(), ref.component->GetClassID()));

    return COM::kOK;
}

