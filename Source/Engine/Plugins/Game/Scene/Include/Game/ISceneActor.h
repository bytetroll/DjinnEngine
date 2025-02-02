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

#include <Game/IActor.h>

namespace Djinn::Game {
    class ITransformComponent;

    class DJINN_EXPORT_PLUGINS_GAME_SCENE ISceneActor : public IActor {
    public:
        DJINN_COM_INTERFACE();

        ISceneActor(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActor(registry, outer, host) {

        }

        /// Overrides
        void StaticInitialize() override;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance*>(this);
                    return COM::kOK;
                case IActor::kIID:
                    *out = static_cast<IActor*>(this);
                    return COM::kOK;
                case ISceneActor::kIID:
                    *out = static_cast<ISceneActor*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Set the transform
        /// \param transform
        void SetTransform(ITransformComponent* transform);

        /// Get the transform component
        /// \return
        ITransformComponent *GetTransform() const {
            return transform;
        }

    private:
        /// Root transform component
        Core::WeakPtr<ITransformComponent> transform;
    };
}