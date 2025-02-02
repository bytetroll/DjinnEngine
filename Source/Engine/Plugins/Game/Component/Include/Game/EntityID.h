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