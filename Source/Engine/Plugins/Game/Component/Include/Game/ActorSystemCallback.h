//
// Created by Dev on 8/18/2018.
//
#pragma once

#include <Core/Delegate.h>
#include "ActorCallbackStage.h"

namespace Djinn::Game {
    class ActorSystemCallback {
    public:
        ActorSystemCallback(std::nullptr_t = nullptr) {}

        /// Validity operator
        /// \return
        operator bool() const {
            return callback.IsValid();
        }

        /// The registered stage
        ActorCallbackStage stage = kDefaultActorCallback;

        /// Update callback
        Core::Delegate<void(double)> callback;

        /// Free helper
        Core::VoidDelegate deleter;
    };
}