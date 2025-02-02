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

#include <Game/IWorldSystem.h>
#include <Core/ArrayView.h>
#include "EntityID.h"
#include "Entity.h"

namespace Djinn::Base {
    class IPipeline;
}

namespace Djinn::Game {
    class IEntityComponent;
    class IEntityComponentHost;

    struct DeferredEntityEvent {
        EntityID id;
        Core::DynamicDelegate delegate;
        Core::DynamicParameterStack stack;
    };

    class IEntitySystem : public IWorldSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IEntitySystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldSystem(registry, outer, host) {
            // ...
        }

        /// Register a component system
        /// \param system
        /// \return
        virtual COM::Result Register(IEntityComponent *system) = 0;

        /// Deregister a component system
        /// \param system
        /// \return
        virtual COM::Result Deregister(IEntityComponent *system) = 0;

        /// Queue an action
        /// \param system
        /// \return
        virtual COM::Result Queue(const DeferredEntityEvent &event) = 0;

        /// Queue an action
        /// \param system
        /// \return
        virtual COM::Result Queue(IEntityComponent *system, const DeferredEntityEvent &event) = 0;

        /// Get pipeline
        /// \return
        virtual Base::IPipeline* GetPipeline() = 0;

        /// Create an entity
        /// \param out
        /// \return
        virtual COM::Result CreateEntity(EntityReference *out) = 0;

        /// Destroy an entity
        /// \param id
        /// \return
        virtual COM::Result DestroyEntity(const EntityReference &id) = 0;

        /// Create a component
        /// \param entity
        /// \param ccid
        /// \param out
        /// \return
        virtual COM::Result CreateComponent(const EntityReference& entity, COM::ClassID ccid, EntityComponentReference* out) = 0;

        /// Destroy a component
        /// \param component
        /// \param id
        /// \return
        virtual COM::Result DestroyComponent(const EntityComponentReference& ref) = 0;

        /// Enable a component
        /// \param component
        /// \param id
        /// \return
        virtual COM::Result EnableComponent(const EntityComponentReference& ref) = 0;

        /// Disable a component
        /// \param component
        /// \param id
        /// \return
        virtual COM::Result DisableComponent(const EntityComponentReference& ref) = 0;

        /// Get the system that manages given component class id
        /// \param ccid
        /// \return
        virtual COM::Result GetComponentSystem(const COM::ClassID &ccid, IEntityComponent **out) = 0;

        /// Get commit index
        /// \return
        virtual Int64 *GetCommitHandle() = 0;

        /// Entity wrapper creator
        COM::Result CreateEntity(Entity &out) {
            EntityReference ref;
            if (!CreateEntity(&ref)) {
                return COM::kError;
            }

            out = Entity(ref);
            return COM::kOK;
        }
    };
}