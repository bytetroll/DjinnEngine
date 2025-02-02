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
 on 2/28/2018.
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