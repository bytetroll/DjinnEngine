//
// Created by Dev on 20/06/2018.
//

#pragma once

#include <COM/Result.h>
#include "EntityID.h"

namespace Djinn::Game {
    class IEntityComponentState;

    class IEntityComponentHost {
    public:
        /// Create a component
        /// \param eid
        /// \param component
        /// \param out
        /// \return
        virtual COM::Result CreateComponent(const EntityComponentReference& ref) = 0;

        /// Destroy a component
        /// \param id
        /// \return
        virtual COM::Result DestroyComponent(const EntityID &id) = 0;

        /// Get the pointer of a component
        /// \param cid
        /// \param out
        /// \return
        virtual COM::Result GetComponentPtr(const EntityID &id, IEntityComponentState **out) = 0;

        /// Enable a component
        /// \param id
        /// \return
        virtual COM::Result Enable(const EntityID &id) = 0;

        /// Disable a component
        /// \param id
        /// \return
        virtual COM::Result Disable(const EntityID &id) = 0;

        /// Apply any deferred changes
        /// \return
        virtual COM::Result ApplyChanges() = 0;
    };
}