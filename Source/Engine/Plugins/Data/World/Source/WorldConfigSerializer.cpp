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
 on 3/1/2018.
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
