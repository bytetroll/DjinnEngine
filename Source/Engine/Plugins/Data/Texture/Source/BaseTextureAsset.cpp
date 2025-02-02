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
