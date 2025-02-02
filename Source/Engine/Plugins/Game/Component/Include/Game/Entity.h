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

#include "EntityID.h"
#include "EntityComponent.h"
#include <COM/ID.h>
#include <Core/Exception.h>

namespace Djinn::Game {
    class IEntitySystem;

    /// Declare common exceptions
    /// Note: Failure in the entity wrapper is considered fatal
    DJINN_EXCEPTION(ComponentCreationFailed, "Entity component creation failed");

    /// Entity wrapper
    struct DJINN_EXPORT_PLUGINS_GAME_COMPONENT Entity {
        Entity(std::nullptr_t = nullptr) {

        }

        /// Construct
        /// \param id
        /// \param system
        Entity(const EntityReference& ref) : ref(ref) {

        }

        /// Create and add a component
        /// \tparam T
        /// \return
        template<typename T>
        EntityComponent<T> AddComponent() const {
            return EntityComponent<T>(AddComponent(T::kCID));
        }

        /// Create and add a component
        /// \param cid
        /// \return
        OpaqueEntityComponent AddComponent(const COM::ClassID &cid) const;

        /// Get a component
        /// \tparam T
        /// \return
        template<typename T>
        EntityComponent<T> GetComponent() const {
            return EntityComponent<T>(GetComponent(T::kCID));
        }

        /// Get a component or add it
        /// \tparam T
        /// \return
        template<typename T>
        EntityComponent<T> GetOrAddFirstComponent() const {
            auto com = GetComponent<T>();
            if (!com) {
                return AddComponent<T>();
            }
            return com;
        }

        /// Get a component
        /// \param cid
        /// \return
        OpaqueEntityComponent GetComponent(const COM::ClassID &cid) const;

        /// Remove first component
        /// \tparam T
        template<typename T>
        bool RemoveComponent() const {
            auto com = GetComponent<T>();
            if (!com) {
                return false;
            }
            return com.Destroy();
        }

        /// Remove component
        /// \tparam T
        template<typename T>
        bool RemoveComponent(EntityComponent<T> &com) const {
            return com.Destroy();
        }

        /// Get the reference
        /// \return
        const EntityReference& GetReference() const {
            return ref;
        }

        /// Destroy this entity
        /// \return
        bool Destroy() const;

        /// Valid entity?
        /// \return
        operator bool() const {
            return ref.system != nullptr;
        }

    private:
        EntityReference ref;
    };
}