//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <Core/DynamicDelegate.h>
#include <Core/Array.h>
#include <Core/WeakPtr.h>
#include "IActorSystemInstance.h"

namespace Djinn::Game {
    /// Detail
    namespace Detail {
        DJINN_EXPORT_PLUGINS_GAME_COMPONENT void QueueActorEvent(const Core::WeakPtr<IActorSystemInstance>& instance, const Core::DynamicDelegate &delegate, const Core::DynamicParameterStack &stack);
    }

    template<typename T>
    class ActorEvent {
    public:
        /// Add an event
        void Add(IActorSystemInstance* instance, const T& action) {
            connections.Add({instance->GetWeakPtr(), action});
        }

        /// Signal all events
        /// \tparam A
        /// \param args
        template<typename... A>
        void Signal(IActorSystemInstance* sender, A&&... args) {
            // Remove missing connections
            connections.RemoveAll([&](Connection& conn) {
                if (conn.instance) {
                    Detail::QueueActorEvent(conn.instance, conn.delegate, Core::Pack(args...));
                    return false;
                }
                return true;
            });
        }

        /// Clear all events
        void Clear() {
            connections.Clear();
        }

    private:
        struct Connection {
            Core::WeakPtr<IActorSystemInstance> instance;
            T delegate;
        };

        Core::Array<Connection> connections;
    };
}