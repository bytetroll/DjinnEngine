//
// Created by Dev on 8/20/2018.
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