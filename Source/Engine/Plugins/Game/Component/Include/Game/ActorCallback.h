//
// Created by Dev on 8/18/2018.
//
#pragma once

#include "ActorSystemCallback.h"

namespace Djinn::Game {
    class IActorSystemInstance;

    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT ActorCallback {
    public:
        ~ActorCallback();

        /// Set the stage
        /// \param stage
        void SetStage(const ActorCallbackStage &stage);

        /// Set the callback
        /// \param stage
        void SetCallback(const Core::Delegate<void(double)> &callback);

        /// Register this callback
        /// \param instance
        void Register(IActorSystemInstance *instance);

        /// Deregister this callback
        void Deregister();

        /// Get the current registered instance
        /// \return
        IActorSystemInstance *GetInstance() const {
            return instance;
        }

        /// Get the current stage
        /// \return
        const ActorCallbackStage &GetStage() const {
            return systemCallback.stage;
        }

    private:
        /// The currently registered instance
        IActorSystemInstance *instance = nullptr;

        /// The system callback
        ActorSystemCallback systemCallback;
    };
}