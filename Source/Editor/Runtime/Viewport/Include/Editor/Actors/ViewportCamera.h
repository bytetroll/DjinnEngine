//
// Created by Dev on 7/13/2018.
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