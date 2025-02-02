//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <Core/Delegate.h>
#include <Core/Array.h>
#include "EntityComponent.h"
#include "IEntityComponentState.h"

namespace Djinn::Game {
    /// Detail
    namespace Detail {
        DJINN_EXPORT_PLUGINS_GAME_COMPONENT void QueueFreeEntityEvent(const OpaqueEntityComponent &sender, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack);
        DJINN_EXPORT_PLUGINS_GAME_COMPONENT void QueueEntityEvent(const OpaqueEntityComponent &opaque, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack);
    }

    template<typename T>
    class EntityEvent {
    public:
        /// Add an event
        void Add(const OpaqueEntityComponent& component, const T& action) {
            connections.Add({component, action});
        }

        /// Add a free event
        void AddFree(const T& action) {
            frees.Add(action);
        }

        /// Signal all events
        /// \tparam A
        /// \param args
        template<typename... A>
        void Signal(const OpaqueEntityComponent& sender, A&&... args) {
            // Remove missing connections
            connections.RemoveAll([&](Connection& conn) {
                if (conn.com) {
                    Detail::QueueEntityEvent(conn.com, conn.delegate, Core::Pack(args...));
                    return false;
                }
                return true;
            });

            // Submit free
            for (auto&& free : frees) {
                Detail::QueueFreeEntityEvent(sender, free, Core::Pack(args...));
            }
        }

        /// Clear all events
        void Clear() {
            connections.Clear();
        }

    private:
        struct Connection {
            OpaqueEntityComponent com;
            T delegate;
        };

        Core::Array<Connection> connections;
        Core::Array<T> frees;
    };
}