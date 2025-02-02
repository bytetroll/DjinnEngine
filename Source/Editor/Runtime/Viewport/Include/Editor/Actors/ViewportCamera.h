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
 on 7/13/2018.
//
#pragma once

#include "IViewportCamera.h"
#include <Math/Vector3.h>
#include <Game/ActorCallback.h>

namespace Djinn::Graphics {
    class IOutput;
}

namespace Djinn::Game {
    class InputComponent;
    class Camera;
}

namespace Djinn::Editor {
    class ViewportCamera : public IViewportCamera {
    public:
        DJINN_COM_CLASS();

        ViewportCamera(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        void Created() override;
        void Update(double delta);
        Game::ICamera *GetGameCamera() override;

    public:
        /// Set the output of this camera
        /// \param output
        void SetOutput(Graphics::IOutput *output);

    private:
        Game::InputComponent *input = nullptr;
        Game::Camera *camera;
        Game::ActorCallback callback;

        /// Immediate
        Math::Vector3 direction;
        Math::Vector2 rotation;

        /// Velocities
        Math::Vector3 positionVelocity;
        Math::Vector2 rotationVelocity;

        /// Current camera angles
        Math::Vector2 angles;

        /// Is this camera captured?
        bool isCaptured = false;
    };
}