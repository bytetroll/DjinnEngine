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

#include <Serialization/SerializerRegistry.h>
#include <Serialization/ISerializer.h>

DJINN_NS2(Serialization);

SerializerRegistry::SerializerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializerRegistry(registry, outer, host) {

}

COM::Result SerializerRegistry::AddSerializer(const COM::ClassID &id, ISerializer *serializer) {
    if (!serializer) {
        return COM::kInvalidParameter;
    }

    serializers[serializer->GetClassID()] = serializer;
    mapped[id] = serializer;
    return COM::kOK;
}

COM::Result SerializerRegistry::RemoveSerializer(const COM::ClassID &id) {
    mapped.Remove(id);
    return COM::kOK;
}

COM::Result SerializerRegistry::GetSerializer(const COM::ClassID &id, ISerializer** out) {
    auto&& it = mapped.Find(id);
    if (it == mapped.End()) {
        return kNoSerializer;
    }

    *out = (*it).second;
    return COM::kOK;
}

COM::Result SerializerRegistry::GetSerializerWithID(const COM::ClassID &id, ISerializer **out) {
    auto&& it = serializers.Find(id);
    if (it == serializers.End()) {
        return kNoSerializer;
    }

    *out = (*it).second;
    return COM::kOK;
}

COM::Result SerializerRegistry::AddSerializer(ISerializer *serializer) {
    serializers[serializer->GetClassID()] = serializer;
    return COM::kOK;
}

COM::Result SerializerRegistry::AddSerializer(const COM::ClassID &id, const COM::ClassID &serializerID) {
    auto&& it = serializers.Find(serializerID);
    if (it == serializers.end()) {
        return COM::kError;
    }

    mapped[id] = (*it).second;
    return COM::kOK;
}
