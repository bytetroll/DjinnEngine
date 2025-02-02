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