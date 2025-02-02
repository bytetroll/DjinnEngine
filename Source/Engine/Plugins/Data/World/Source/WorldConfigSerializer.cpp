//
// Created by Dev on 3/1/2018.
//

#include <Data/WorldConfigSerializer.h>
#include <Data/WorldConfig.h>
#include <Host/IRegistry.h>
#include <Data/IWorldSetting.h>

DJINN_NS2(Data);

COM::Result WorldConfigSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == WorldConfig::kCID);

    // ...
    auto asset = static_cast<WorldConfig *>(object);

    // Write header
    archive << asset->settings.Size();

    // Read settings
    for (auto kv : asset->settings) {
        archive << kv.first.Get();
        if (!(archive << kv.second)) {
            return COM::kError;
        }
    }

    // OK
    return COM::kOK;
}

COM::Result WorldConfigSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    WorldConfig *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}


COM::Result WorldConfigSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<WorldConfig*>(object);

    // Read header
    USize size;
    archive >> size;

    // Read settings
    for (USize i = 0; i < size; i++) {
        COM::ClassID::Type cid;
        archive >> cid;

        Data::IWorldSetting *setting;
        if (!(archive >> &setting)) {
            return COM::kError;
        }

        asset->settings[COM::ClassID(cid)] = setting;
    }

    // OK
    return COM::kOK;
}
