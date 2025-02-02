//
// Created by Dev on 10/19/2017.
//

#include <Serialization/SerializerHost.h>
#include <Host/Registry.h>
#include <Serialization/SerializerRegistry.h>
#include <Serialization/ISerializer.h>
#include <Base/DefaultLogs.h>
#include <Core/Assert.h>

DJINN_NS2(Serialization);

SerializerHost::SerializerHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializerHost(registry, outer, host) {

}

COM::Result SerializerHost::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&serializerRegistry)) {
        ErrorLog(this).Write("Could not fetch serializer registry from registry, serialization worldHost will not function");
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result SerializerHost::Serialize(Archive &archive, COM::IUnknown *object) {
    ISerializer *serializer;
    if (!serializerRegistry->GetSerializer(object->GetClassID(), &serializer)) {
        ErrorLog(this).Write("No serializer for object ", object->GetClassName(), ", serialization failed");
        return COM::kError;
    }

    // Write header
    archive << serializer->GetClassID().Get();
    USize sizeStart = archive.GetStream()->GetPosition();
    archive << USize(0);

    // Serialize
    if (!serializer->Serialize(archive, object)) {
        ErrorLog(this).Write("Serializer failed with class ", serializer->GetClassName());
        return COM::kError;
    }

    // Mark offset
    USize blockEnd = archive.GetStream()->GetPosition();
    archive.GetStream()->SetPosition(sizeStart);
    archive << blockEnd;
    archive.GetStream()->SetPosition(blockEnd);

    // OK
    return COM::kOK;
}

COM::Result SerializerHost::Deserialize(Archive &archive, COM::IUnknown **out) {
    // Read header
    COM::ClassID::Type serializerID;
    archive >> serializerID;
    USize blockEnd;
    archive >> blockEnd;

    // Must never be zero
    if (blockEnd == 0) {
        ErrorLog(this).Write("Incorrect block ending for serializer ", serializerID, ", serialization failed");
        return COM::kError;
    }

    // Get serializer with id
    ISerializer *serializer;
    if (!serializerRegistry->GetSerializerWithID(COM::ClassID(serializerID), &serializer)) {
        ErrorLog(this).Write("No serializer with id ", serializerID, ", serialization failed");
        return COM::kError;
    }

    // Deserialize
    if (!serializer->Deserialize(archive, out)) {
        ErrorLog(this).Write("Failed to deserialize with id ", serializerID, ", skipping block");

        // Skip to end block
        archive.GetStream()->SetPosition(blockEnd);

        // ...
        return COM::kError;
    }

    return COM::kOK;
}

COM::Result SerializerHost::Deserialize(Archive &archive, COM::IUnknown *object) {
    // Read header
    COM::ClassID::Type serializerID;
    archive >> serializerID;
    USize blockEnd;
    archive >> blockEnd;

    // Must never be zero
    if (blockEnd == 0) {
        ErrorLog(this).Write("Incorrect block ending for serializer ", serializerID, ", serialization failed");
        return COM::kError;
    }

    // Get serializer with id
    ISerializer *serializer;
    if (!serializerRegistry->GetSerializerWithID(COM::ClassID(serializerID), &serializer)) {
        ErrorLog(this).Write("No serializer with id ", serializerID, ", serialization failed");
        return COM::kError;
    }

    // Deserialize
    if (!serializer->Deserialize(archive, object)) {
        ErrorLog(this).Write("Failed to deserialize with id ", serializerID, ", skipping block");

        // Skip to end block
        archive.GetStream()->SetPosition(blockEnd);

        // ...
        return COM::kError;
    }

    return COM::kOK;
}

