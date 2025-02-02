//
// Created by Dev on 1/17/2018.
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