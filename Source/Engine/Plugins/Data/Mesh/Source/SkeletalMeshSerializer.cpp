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
 on 10/18/2017.
//

#include <Data/SkeletalMeshSerializer.h>
#include <Core/Assert.h>
#include <Data/SkeletalMeshAsset.h>
#include <Host/IRegistry.h>

DJINN_NS2(Data);

SkeletalMeshSerializer::SkeletalMeshSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMeshSerializer(registry, outer, host) {

}


COM::Result SkeletalMeshSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == SkeletalMeshAsset::kCID);

    // ...
    auto asset = static_cast<SkeletalMeshAsset*>(object);

    // Write header
    archive << asset->indexStream.indices.Size() << asset->vertexStream.vertices.Size();

    // Write indices
    for (USize i = 0; i < asset->indexStream.indices.Size(); i++) {
        archive << asset->indexStream.indices[i];
    }

    // Write vertices
    for (USize i = 0; i < asset->vertexStream.vertices.Size(); i++) {
        archive << asset->vertexStream.vertices[i];
    }
    return COM::kOK;
}

COM::Result SkeletalMeshSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    SkeletalMeshAsset* asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}

COM::Result SkeletalMeshSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<SkeletalMeshAsset*>(object);

    // Read header
    USize indices;
    archive >> indices;
    USize vertices;
    archive >> vertices;

    // Read indices
    asset->indexStream.indices.Resize(indices);
    for (USize i = 0; i < indices; i++) {
        archive >> asset->indexStream.indices[i];
    }

    // Read vertices
    asset->vertexStream.vertices.Resize(vertices);
    for (USize i = 0; i < vertices; i++) {
        archive >> asset->vertexStream.vertices[i];
    }

    return COM::kOK;
}
