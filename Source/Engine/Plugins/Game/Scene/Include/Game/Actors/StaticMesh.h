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
#include <Game/Proxies/ISceneMeshProxy.h>

namespace Djinn::Data {
    class MeshCollectionAsset;
    class IMaterialAsset;
}

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE StaticMesh : public ISceneActor, public ISceneMeshProxy {
    public:
        DJINN_COM_CLASS();

        StaticMesh(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneActor(registry, outer, host) {

        }

        /// Overrides
        void Created() override;
        void BuildScene(SceneBuilder &bucket) override;
        void Build(const Core::ArrayView<SceneObject> &requests, Core::Buffer<Graphics::SceneMeshPacket>& buffer) override;

    public:
        /// Set the mesh asset
        /// \param mesh
        void SetMesh(const Data::Asset<Data::MeshCollectionAsset>& mesh) {
            this->mesh = mesh;
        }

        /// Set the material asset
        /// \param material
        void SetMaterial(const Data::Asset<Data::IMaterialAsset>& material) {
            this->material = material;
        }

        /// Get the current mesh
        /// \return
        const Data::Asset<Data::MeshCollectionAsset>& GetMesh() const {
            return mesh;
        }

        /// Get the current material
        /// \return
        const Data::Asset<Data::IMaterialAsset>& GetMaterial() const {
            return material;
        }

    protected:
        /// Mesh asset
        Data::Asset<Data::MeshCollectionAsset> mesh = nullptr;

        /// Material asset
        Data::Asset<Data::IMaterialAsset> material = nullptr;
    };
}