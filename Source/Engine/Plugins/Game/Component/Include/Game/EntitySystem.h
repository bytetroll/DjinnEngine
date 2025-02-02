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
#pragma once

#include <Core/HashMap.h>
#include <Core/IndexArray.h>
#include <Async/Event.h>
#include <Core/Buffer.h>
#include <Core/SlotArray.h>
#include <Core/SparseArray.h>
#include <Platform/RecursiveSpinMutex.h>
#include "IEntitySystem.h"

namespace Djinn::Base {
    class Pipeline;
}

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT EntitySystem : public IEntitySystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        EntitySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this entity system
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result Run(StepMode mode, double delta) override;
        StepModeSet GetStepMode() override;
        COM::Result Register(IEntityComponent *system) override;
        COM::Result Deregister(IEntityComponent *system) override;
        Base::IPipeline *GetPipeline() override;
        COM::Result CreateEntity(EntityReference *out) override;
        COM::Result DestroyEntity(const EntityReference &id) override;
        COM::Result CreateComponent(const EntityReference &entity, COM::ClassID ccid, EntityComponentReference *out) override;
        COM::Result DestroyComponent(const EntityComponentReference &ref) override;
        COM::Result EnableComponent(const EntityComponentReference &ref) override;
        COM::Result DisableComponent(const EntityComponentReference &ref) override;
        COM::Result GetComponentSystem(const COM::ClassID &ccid, IEntityComponent **out) override;
        COM::Result Configure(Base::PipelineBuilder& builder) override;
        COM::Result Queue(IEntityComponent *system, const DeferredEntityEvent &event) override;
        COM::Result Queue(const DeferredEntityEvent &event) override;
        Int64 *GetCommitHandle() override;

    private:
        struct EntityMeta {
            USize cid = 0;
            bool deleted = false;
        };

        struct ComponentMeta {
            Platform::RecursiveSpinMutex mutex;
            Core::Array<DeferredEntityEvent> events;
            Core::Array<DeferredEntityEvent> stagedEvents;
        };

        Platform::RecursiveSpinMutex freeEventMutex;
        Core::Array<DeferredEntityEvent> freeEvents;
        Core::Array<DeferredEntityEvent> stagedFreeEvents;

        Core::HashMap<IEntityComponent *, ComponentMeta> componentMetas;

        Core::SlotArray<EntityMeta> entities;

        /// Index array
        Core::SparseArray<USize> indices;

        Core::AtomicCounter entityCounter;

        Core::HashMap<COM::ClassID, IEntityComponent *> componentMap;
        Core::Array<IEntityComponent *> components;
        Base::Pipeline *pipeline;

        Platform::RecursiveSpinMutex entityMutex;
        Core::Buffer<EntityID> deletions;

        Int64 commitIndex = 0;
    };
}