//
// Created by Dev on 8/25/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include "SceneBuilder.h"

namespace Djinn::Game {
    class ISceneProxy {
    public:
        DJINN_COM_INTERFACEID();

        /// Build objects
        /// \param bucket
        virtual void BuildScene(SceneBuilder &bucket) = 0;

        /// Get the interface id of this proxy
        /// \return
        virtual COM::InterfaceID GetInterfaceID() = 0;

    public:
        /// Request a static update
        void RequestStaticUpdate() {
            staticRequest = true;
        }

        /// Request a dynamic update
        void SetDynamicPolicy(bool enabled) {
            dynamicPolicy = enabled;
        }

        /// Remove the static request
        /// \param data
        void RemoveStaticRequest() {
            staticRequest = false;
        }

        /// Set the structure state
        /// \param data
        void SetStructureState(Int64 data) {
            structureState = data;
        }

        /// Get the static state
        /// \return
        Int64 GetStructureState() const {
            return structureState;
        }

        /// Does this proxy
        /// \return
        bool HasStaticRequest() const {
            return staticRequest;
        }

        /// Get the dynamic policy
        /// \return
        bool GetDynamicPolicy() const {
            return dynamicPolicy;
        }

    private:
        /// Does this proxy have a static request
        bool staticRequest = true;

        /// Dynamic policy
        bool dynamicPolicy = false;

        /// Current states
        Int64 structureState = -1;
    };
}
