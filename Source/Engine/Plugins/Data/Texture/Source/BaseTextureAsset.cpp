//
// Created by Dev on 1/28/2018.
//

#include <Data/BaseTextureAsset.h>

DJINN_NS2(Data);

BaseTextureAsset::BaseTextureAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITextureAsset(registry, outer, host) {

}


GHA::Format BaseTextureAsset::GetFormat() {
    return format;
}

UInt32 BaseTextureAsset::GetMipCount() {
    return static_cast<UInt32>(mipMapBlobs.Size());
}

GHA::Size BaseTextureAsset::GetSize() {
    return size;
}

void BaseTextureAsset::GetMipStreamables(USize &outCount, GHA::IStreamable **outStreamables) {
    if (!outStreamables) {
        outCount = mipMapBlobs.Size();
    } else {
        for (USize i = 0; i < outCount; i++) {
            outStreamables[i] = &mipMapBlobs[i];
        }
    }
}

COM::Result BaseTextureAsset::MipMapStream::Enumerate(USize &size, void **blob) {
    size = data.ByteSize();
    *blob = data.Ptr();
    return COM::kOK;
}
