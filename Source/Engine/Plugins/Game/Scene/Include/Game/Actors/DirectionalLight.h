//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Game/ISceneActor.h>
#include <Data/Asset.h>
#include <Game/Proxies/ISceneDirectionalLightProxy.h>

namespace Djinn::Data {
    class MeshCollectionAsset;
    class IMaterialAsset;
}

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE DirectionalLight : public ISceneActor, public ISceneDirectionalLightProxy {
    public:
        DJINN_COM_CLASS();

        DirectionalLight(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneActor(registry, outer, host) {

        }

        /// Overrides
        void Created() override;
        void BuildScene(SceneBuilder &bucket) override;
        void Build(const Core::ArrayView<SceneObject> &requests, Graphics::SceneDirectionalLightPacketBuffer *buffer) override;

    public:
        /// Set the color
        /// \param color
        void SetColor(const Math::Vector3 &color) {
            this->color = color;
        }

        /// Set the intensity
        /// \param intensity
        void SetIntensity(float intensity) {
            this->intensity = intensity;
        }

        /// Get the color
        /// \return
        const Math::Vector3 &GetColor() const {
            return color;
        }

        /// Get the intensity
        /// \return
        float GetIntensity() const {
            return intensity;
        }

    protected:
        /// Color
        Math::Vector3 color = Math::Vector3(1, 1, 1);

        /// Intensity
        float intensity = 1.0f;
    };
}