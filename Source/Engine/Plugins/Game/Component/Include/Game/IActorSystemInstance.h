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
 on 8/19/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Game {
    class IActorSystem;
    class IScene;
    class IWorld;

    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT IActorSystemInstance : public COM::TUnknown<IActorSystemInstance> {
    public:
        DJINN_COM_INTERFACE();

        IActorSystemInstance(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this instance
        /// \param system
        void InitializeSystemInstance(IActorSystem* system, IScene* scene);

        /// Get the system that owns this actor
        /// \return
        IActorSystem* GetSystem() const {
            return system;
        }

        /// Get the scene that owns this actor
        /// \return
        IScene* GetScene() const {
            return scene;
        }

        /// Get the world that owns this actor
        /// \return
        IWorld* GetWorld() const {
            return world;
        }

        /// Is this instance marked for deletion?
        /// \return
        bool IsMarkedForDeletion() const {
            return isDeleted;
        }

    protected:
        /// System that owns this instance
        IActorSystem* system = nullptr;

        /// Scene this actor belongs to
        IScene* scene = nullptr;

        /// World this actor belongs to
        IWorld* world = nullptr;

        /// Is this instance marked for deletion?
        bool isDeleted = false;
    };
}