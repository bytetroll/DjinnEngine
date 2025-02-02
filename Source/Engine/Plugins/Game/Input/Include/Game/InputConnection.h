//
// Created by Dev on 1/16/2018.
//
#pragma once

#include <Core/Delegate.h>

namespace Djinn::Game {
    using InputConnectionDestroyDelegate = Core::Delegate<void()>;

    /// Input connection lifetime wrapper
    struct InputConnection {
        InputConnection() = default;

        /// Deconstruct
        ~InputConnection() {
            if (!manual) {
                Unbind();
            }
        }

        /// Assign this connection
        /// \param destroy
        void Assign(const InputConnectionDestroyDelegate& destroy) {
            this->destroy = destroy;
            manual = false;
        }

        /// Set this connection as manual unbinding
        /// \param b
        void Manual(bool b = true) {
            manual = b;
        }

        /// Manually unbind this connection
        void Unbind() {
            if (destroy.IsValid()) {
                destroy();
                destroy = {};
            }
        }

    private:
        bool manual = false;
        InputConnectionDestroyDelegate destroy;
    };
}