//
// Created by Dev on 10/18/2017.
//

#include <Data/BaseMeshSerializer.h>
#include <Core/Assert.h>
#include <Data/BaseMeshAsset.h>
#include <Host/IRegistry.h>

DJINN_NS2(Data);

BaseMeshSerializer::BaseMeshSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMeshSerializer(registry, outer, host) {

}


COM::Result BaseMeshSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == BaseMeshAsset::kCID);

    // ...
    auto asset = static_cast<BaseMeshAsset*>(object);

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

COM::Result BaseMeshSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    BaseMeshAsset* asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}

COM::Result BaseMeshSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<BaseMeshAsset*>(object);

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
