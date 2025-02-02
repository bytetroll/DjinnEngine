//
// Created by Dev on 4/5/2018.
//

#include <Data/BaseMaterialSerializer.h>
#include <Data/BaseMaterialAsset.h>
#include <Data/ShaderAsset.h>
#include <Host/IRegistry.h>

DJINN_NS2(Data);

BaseMaterialSerializer::BaseMaterialSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialSerializer(registry, outer, host) {

}

COM::Result BaseMaterialSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == BaseMaterialAsset::kCID);

    // ...
    auto asset = reinterpret_cast<BaseMaterialAsset *>(object);

    // Write header
    archive << asset->shaders.Size();

    // Write shaders
    for (const auto& kv : asset->shaders) {
        archive << kv.first;

        // Serialize shader
        if (!(archive << kv.second)) {
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result BaseMaterialSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    BaseMaterialAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}

COM::Result BaseMaterialSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<BaseMaterialAsset*>(object);

    // Read header
    USize shaderCount;
    archive >> shaderCount;

    // Read shaders
    for (USize i = 0; i < shaderCount; i++) {
        UInt64 key;
        archive >> key;

        // Deserialize shader
        ShaderAsset* shader;
        if (!(archive >> &shader)) {
            return COM::kError;
        }

        // Insert
        asset->shaders.Add(key, shader);
    }

    // OK
    return COM::kOK;
}
