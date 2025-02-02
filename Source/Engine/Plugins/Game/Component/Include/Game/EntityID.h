//
// Created by Dev on 1/17/2018.
//
#pragma once

#include <Core/Types.h>
#include <Core/Size.h>
#include <Core/Hash.h>

namespace Djinn::Game {
    class IEntitySystem;
    class IEntityComponent;
    class IEntityComponentHost;

    struct EntityID {
        bool operator==(const EntityID &other) const {
            return id == other.id;
        }

        bool operator!=(const EntityID &other) const {
            return id != other.id;
        }

        USize id;
    };

    struct EntityReference {
        EntityReference() {

        }

        EntityReference(const EntityID &id, IEntitySystem *system) : id(id), system(system) {

        }

        bool operator==(const EntityReference &other) const {
            return id == other.id && system == other.system;
        }

        bool operator!=(const EntityReference &other) const {
            return id != other.id || system != other.system;
        }

        EntityID id;
        IEntitySystem *system = nullptr;
    };

    struct EntityComponentReference {
        EntityComponentReference() {

        }

        EntityComponentReference(const EntityReference &entity, IEntityComponent *component, IEntityComponentHost *host) : entity(entity), component(component), host(host) {

        }

        bool operator==(const EntityComponentReference &other) const {
            return entity == other.entity && component == other.component && host == other.host;
        }

        bool operator!=(const EntityComponentReference &other) const {
            return entity != other.entity || component != other.component || host != other.host;
        }

        EntityReference entity;
        IEntityComponent *component = nullptr;
        IEntityComponentHost *host = nullptr;
    };
}

namespace Djinn {
    /// Common hashes
    template<>
    struct Hasher<Game::EntityID> {
        USize operator()(const Game::EntityID &value) const { return Hasher<USize>{}(value.id); }
    };
}