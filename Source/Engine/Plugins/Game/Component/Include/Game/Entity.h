//
// Created by Dev on 1/17/2018.
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