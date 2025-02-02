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

#include <Serialization/ObjectSerializer.h>
#include <Host/IRegistry.h>
#include <Core/Common.h>
#include <Base/DefaultLogs.h>
#include <Reflection/IClassInfo.h>
#include <Reflection/IMemberInfo.h>
#include <Core/Hash.h>
#include <Serialization/SerializerHost.h>
#include <Reflection/IMemberInfo.h>

DJINN_NS2(Serialization);

/* Number serialization layout:
 * TopClassID ClassCount
 *    <Class>...
 * ...
 * */

/* Class serialization layout:
 * ClassID SerializedFieldsCount
 *    ClassID NameID Data
 *    ...
 * ...
 * */

ObjectSerializer::ObjectSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializer(registry, outer, host) {
}

COM::Result ObjectSerializer::Initialize() {
    if (!registry->GetPipeline()->GetInterface(&reflection)) {
        ErrorLog(this).Write("Reflection registry not attached, object serialization will not be functional");
        return COM::kError;
    }
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Serializer worldHost not attached, object serialization will not be functional");
        return COM::kError;
    }
    return COM::kOK;
}

COM::Result ObjectSerializer::Serialize(Archive &archive, COM::IUnknown *object) {
    // Get id
    COM::ClassID id = object->GetClassID();

    // Attempt to get class
    Reflection::IClassInfo *_class;
    if (!reflection->GetClass(Reflection::ToID(id), &_class)) {
        ErrorLog(this).Write("Attempting to serialize an unreflected class ", object->GetClassName());
        return COM::kError;
    }

    // Write initial header
    archive << id.Get() << GetClassCount(_class);

    // Serialize
    return SerializeClasses(archive, _class, object);
}

COM::Result ObjectSerializer::Deserialize(Archive &archive, COM::IUnknown **outInstance) {
    // Read header
    COM::ClassID::Type id;
    archive >> id;
    USize count;
    archive >> count;

    // ...
    COM::ClassID classID(id);

    // Create class instance
    void* out;
    if (!registry->CreateClass(classID, nullptr, COM::IUnknown::kIID, &out)) {
        ErrorLog(this).Write("Attempting to deserialize an unregistered class with id ", id);
        return COM::kError;
    }

    // ...
    *outInstance = static_cast<COM::IUnknown*>(out);

    // Deserialize all internal classes
    for (USize i = 0; i < count; i++) {
        if (!DeserializeClass(archive, *outInstance)) {
            return COM::kError;
        }
    }

    return COM::kOK;
}

COM::Result ObjectSerializer::Deserialize(Archive &archive, COM::IUnknown *object) {
    // Read header
    COM::ClassID::Type id;
    archive >> id;
    USize count;
    archive >> count;

    // ClassIDs must match
    if (object->GetClassID().Get() != id) {
        ErrorLog(this).Write("In place object deserialization failed, target object class id mismatch on ", object->GetClassName());
        return COM::kError;
    }

    // Deserialize all internal classes
    for (USize i = 0; i < count; i++) {
        if (!DeserializeClass(archive, object)) {
            return COM::kError;
        }
    }

    return COM::kOK;
}

USize ObjectSerializer::GetClassCount(Reflection::IClassInfo *_class) {
    // Check bases
    USize serializedBases = 1;
    for (auto _base : _class->GetBases()) {
        Reflection::IClassInfo *base;
        if (reflection->GetClass(_base, &base)) {
            serializedBases += GetClassCount(base);
        }
    }

    // Account for self
    return serializedBases;
}

COM::Result ObjectSerializer::SerializeClasses(Archive &archive, Reflection::IClassInfo *_class, COM::IUnknown *object) {
    // Serialize self
    if (!SerializeClass(archive, _class, object)) {
        return COM::kError;
    }

    // Serialize bases
    for (auto _base : _class->GetBases()) {
        Reflection::IClassInfo *base;
        if (reflection->GetClass(_base, &base)) {
            if (!SerializeClasses(archive, base, object)) {
                return COM::kError;
            }
        }
    }
    return COM::kOK;
}


COM::Result ObjectSerializer::SerializeClass(Archive &archive, Reflection::IClassInfo *_class, COM::IUnknown *object) {
    Reflection::ClassID classID = _class->GetClassID();

    // Get members
    auto members = _class->GetMembers();

    // Write initial header
    archive << classID.Get() << members.Size();

    // Write members
    for (USize i = 0; i < members.Size(); i++) {
        auto member = members[i];

        // Get type
        const auto &type = member->GetType();

        // Serialize header
        archive << type.GetHash() << member->GetHash();

        // Primitive?
        if (type.IsPrimitive()) {
            archive.GetStream()->Write(member->GetRaw(object), type.GetByteSize());
        } else {
            COM::IUnknown *com = member->GetCom(object);
            if (!com) {
                WarningLog(this).Write("Member ", member->GetDecoratedName(), " of non primitive or component type is not serializable");
                continue;
            }

            Check(host->Serialize(archive, com));
        }
    }
    return COM::kOK;
}

COM::Result ObjectSerializer::DeserializeClass(Archive &archive, COM::IUnknown *out) {
    // Read header
    USize id;
    archive >> id;
    USize count;
    archive >> count;

    // As class id
    COM::ClassID classID(id);

    // Attempt to get class
    Reflection::IClassInfo *_class;
    if (!reflection->GetClass(Reflection::ToID(classID), &_class)) {
        ErrorLog(this).Write("Attempting to deserialize an unreflected class with id ", id);
        return COM::kError;
    }

    // Read members
    for (USize i = 0; i < count; i++) {
        // Read sub header
        USize typeHash;
        archive >> typeHash;
        USize nameID;
        archive >> nameID;

        // Get target member
        Reflection::IMemberInfo *member = _class->GetMember(nameID);
        if (!member) {
            WarningLog(this).Write("A member with id ", nameID, " is no longer compatible");
            continue;
        }

        // Get type
        const auto &type = member->GetType();
        if (type.GetHash() != typeHash) {
            WarningLog(this).Write("Member ", member->GetDecoratedName(), " is no longer compatible, type signature has changed");
            continue;
        }

        // Primitive?
        if (type.IsPrimitive()) {
            // Read
            archive.GetStream()->Read(member->GetRaw(out), type.GetByteSize());
        } else {
            // Deserialize
            COM::IUnknown *object;
            if (!host->Deserialize(archive, &object)) {
                // Bad serialization will have ignored the entire block
                continue;
            }

            // Assign value
            member->GetDynamic(out).Set(object);
        }
    }
    return COM::kOK;
}
