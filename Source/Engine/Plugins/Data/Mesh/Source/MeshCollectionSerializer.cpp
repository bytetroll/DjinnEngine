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
