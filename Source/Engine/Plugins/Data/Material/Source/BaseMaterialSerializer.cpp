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
 on 4/5/2018.
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
