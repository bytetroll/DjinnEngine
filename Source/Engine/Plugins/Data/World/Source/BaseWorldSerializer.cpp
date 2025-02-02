//
// Created by Dev on 2/28/2018.
//

#include <Data/BaseWorldSerializer.h>
#include <Data/BaseWorldAsset.h>
#include <Host/IRegistry.h>
#include <Data/WorldConfig.h>

DJINN_NS2(Data);

COM::Result BaseWorldSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == BaseWorldAsset::kCID);

    // ...
    auto asset = static_cast<BaseWorldAsset *>(object);

    // Write config
    if (!(archive << asset->config)) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result BaseWorldSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    BaseWorldAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}


COM::Result BaseWorldSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<BaseWorldAsset*>(object);

    // Read config
    if (!(archive >> &asset->config)) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}
