//
// Created by Dev on 8/20/2018.
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