//
// Created by Dev on 8/30/2018.
//

#include <Game/Actors/StaticMesh.h>
#include <Game/ISpatialScene.h>
#include <Game/ActorComponents/ITransformComponent.h>
#include <Graphics/SceneMeshPacket.h>
#include <Data/MeshCollectionAsset.h>
#include <Data/IMaterialAsset.h>

DJINN_NS2(Game);

void StaticMesh::Created() {
    if (auto scene = COM::Cast<ISpatialScene>(GetScene())) {
        // Register proxy
        scene->Register(this);

        // Always update
        SetDynamicPolicy(true);
    }
}

void StaticMesh::BuildScene(SceneBuilder &bucket) {
    bucket.Add(SceneObject().SetIndex(0));
}

void StaticMesh::Build(const Core::ArrayView<SceneObject> &requests, Core::Buffer<Graphics::SceneMeshPacket>& buffer) {
    Graphics::SceneMeshPacket packet;
    packet.transform = GetTransform()->GetWorldTransform();
    packet.material = material;

    if (auto collection = mesh.GetRaw()) {
        for (auto& mesh : collection->meshes) {
            packet.mesh = mesh;
            buffer.Add(packet);
        }
    }
}
