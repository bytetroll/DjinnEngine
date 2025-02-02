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
 on 1/28/2018.
//

#include <Data/BaseTextureSerializer.h>
#include <Data/BaseTextureAsset.h>
#include <Host/IRegistry.h>

DJINN_NS2(Data);

BaseTextureSerializer::BaseTextureSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITextureSerializer(registry, outer, host) {

}


COM::Result BaseTextureSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == BaseTextureAsset::kCID);

    // ...
    auto asset = static_cast<BaseTextureAsset *>(object);

    // Write header
    archive << asset->size.width;
    archive << asset->size.height;
    archive << asset->size.depth;
    archive << static_cast<int>(asset->format);

    // Write count
    archive << asset->mipMapBlobs.Size();

    // Write mip map blobs
    for (auto &blob : asset->mipMapBlobs) {
        // Write count
        archive << blob.data.Size();

        // Write blob
        archive.GetStream()->Write(blob.data.Ptr(), blob.data.ByteSize());
    }

    // OK
    return COM::kOK;
}

COM::Result BaseTextureSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    BaseTextureAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}

COM::Result BaseTextureSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<BaseTextureAsset*>(object);

    // Read header
    archive >> asset->size.width;
    archive >> asset->size.height;
    archive >> asset->size.depth;
    int format;
    archive >> format;
    asset->format = static_cast<GHA::Format >(format);

    // Read count
    USize count;
    archive >> count;

    // Read mip map blobs
    asset->mipMapBlobs.Resize(count);
    for (USize i = 0; i < count; i++) {
        // Read count
        USize blobSize;
        archive >> blobSize;

        // Read blob
        asset->mipMapBlobs[i].data.Resize(blobSize);
        archive.GetStream()->Read(asset->mipMapBlobs[i].data.Ptr(), asset->mipMapBlobs[i].data.ByteSize());
    }

    // OK
    return COM::kOK;
}
