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
 on 8/30/2018.
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
