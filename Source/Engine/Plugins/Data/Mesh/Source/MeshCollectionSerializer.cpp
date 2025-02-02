//
// Created by Dev on 10/18/2017.
//

#include <Data/MeshCollectionSerializer.h>
#include <Core/Assert.h>
#include <Host/IRegistry.h>
#include <Data/MeshCollectionAsset.h>
#include <Data/IMeshAsset.h>

DJINN_NS2(Data);

MeshCollectionSerializer::MeshCollectionSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMeshSerializer(registry, outer, host) {

}


COM::Result MeshCollectionSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == MeshCollectionAsset::kCID);

    // ...
    auto asset = static_cast<MeshCollectionAsset*>(object);

    // Write header
    archive << asset->meshes.Size();

    // Write meshes
    for (USize i = 0; i < asset->meshes.Size(); i++) {
        Check(archive << asset->meshes[i]);
    }
    return COM::kOK;
}

COM::Result MeshCollectionSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    MeshCollectionAsset* asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}

COM::Result MeshCollectionSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<MeshCollectionAsset*>(object);

    // Read header
    USize meshes;
    archive >> meshes;

    // Read meshes
    asset->meshes.Resize(meshes);
    for (USize i = 0; i < meshes; i++) {
        Check(archive >> &asset->meshes[i]);
    }

    return COM::kOK;
}
