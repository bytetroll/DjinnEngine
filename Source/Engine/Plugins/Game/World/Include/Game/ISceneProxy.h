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
 on 8/25/2018.
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
