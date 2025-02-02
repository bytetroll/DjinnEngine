//
// Created by Dev on 10/18/2017.
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
