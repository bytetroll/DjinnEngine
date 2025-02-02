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