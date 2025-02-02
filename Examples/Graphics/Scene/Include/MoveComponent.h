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
 on 10/11/2018.
//
#pragma once

#include <Game/ISceneActor.h>
#include <Game/ActorComponents/InputComponent.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Math/Vector3.h>
#include <Game/ActorCallback.h>

DJINN_NS();

class MoveComponent : public Game::ISceneComponent {
public:
    DJINN_COM_CLASS();

    MoveComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneComponent(registry, outer, host) {

    }

    void Created() override {
        callback.SetCallback(Bindff(Update));
        callback.Register(this);
    }

    void Update(double delta) {
        GetSceneActor()->GetTransform()->SetRotation(Math::Quaternion::RotateRPY({0, 0, static_cast<float>(Math::Cos(elapsed += delta)) * 0.1f}));
    }

private:
    Game::ActorCallback callback;
    double elapsed = 0;
};