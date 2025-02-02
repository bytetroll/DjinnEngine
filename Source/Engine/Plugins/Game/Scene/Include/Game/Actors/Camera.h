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
 on 8/20/2018.
//
#pragma once

#include <Game/ISceneActor.h>
#include <Game/ICamera.h>
#include <Math/Vector3.h>

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE Camera : public ISceneActor, public ICamera {
    public:
        DJINN_COM_CLASS();

        Camera(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void Created() override;
        void Build(Graphics::CameraPacket *packet) override;
        Graphics::IOutput* GetOutput() override;

    public:
        /// Set direction
        /// \param direction
        void SetDirection(const Math::Vector3 &direction);

        /// Lookat point from position
        /// \param eye
        /// \param focus
        void Lookat(const Math::Vector3 &eye, const Math::Vector3 &focus);

        /// Set the output
        /// \param output
        void SetOutput(Graphics::IOutput* output) {
            this->output = output;
        }

    private:
        Graphics::IOutput* output = nullptr;
    };
}