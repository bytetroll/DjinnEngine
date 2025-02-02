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
 on 10/19/2017.
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

