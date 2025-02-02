//
// Created by Dev on 20/06/2018.
//

#pragma once

#include <Game/Entity.h>

namespace Djinn::Game {
    class IEntityComponentState {
    public:
        /// Allow worldHost to write
        friend class IEntityComponentHost;

        /// Get the entity that owns this component
        /// \return
        Entity GetEntity() const {
            return Entity(reference.entity);
        }

        /// Get the entity component that owns this component
        /// \return
        IEntityComponent* GetOwner() const {
            return reference.component;
        }

        /// Get the underlying reference
        /// \return
        const EntityComponentReference& GetReference() const {
            return reference;
        }

    private:
        /// Decoupled reference
        EntityComponentReference reference;
    };
}