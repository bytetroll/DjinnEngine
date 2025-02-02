//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <Core/Delegate.h>
#include <Core/IndexArray.h>

namespace Djinn::Core {
    struct EventConnection {
        /// Construct
        EventConnection() = default;

        /// Assign event
        /// \param deleter
        void Assign(const Core::Delegate<void()>& deleter) {
            this->deleter = deleter;
            manual = true;
        }

        /// Deconstruct, call deleter if valid
        ~EventConnection() {
            if (manual) {
                Release();
            }
        }

        /// Set this connection as manual unbinding
        /// \param b
        void Manual(bool b = true) {
            manual = b;
        }

        /// Manually unbind this connection
        void Release() {
            if (deleter.IsValid()) {
                deleter();
                deleter = {};
            }
        }

    private:
        bool manual = false;

        /// Deleter
        Core::Delegate<void()> deleter;
    };

    template<typename T>
    class EventDispatcher {
    public:
        /// Add an event
        /// \param action
        /// \param connection - May be null
        void Add(const T& action, EventConnection* connection = nullptr) {
            auto idx = delegates.Add(action);
            if (connection) {
                connection->Assign([&, idx] {
                    delegates.Remove(idx);
                });
            }
        }

        /// Signal all events
        /// \tparam A
        /// \param args
        template<typename... A>
        void Signal(A&&... args) {
            for (auto& delegate : delegates) {
                delegate(args...);
            }
        }

        /// Clear all events
        void Clear() {
            delegates.Clear();
        }

        /// Add an event
        /// \param action
        void operator+=(const T& action) {
            Add(action);
        }

    private:
        Core::IndexArray<T> delegates;
    };
}