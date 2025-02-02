#include <Data/ShaderSerializer.h>
#include <Core/Assert.h>
#include <Data/ShaderAsset.h>
#include <Host/IRegistry.h>
#include <Serialization/Common/Containers.h>

DJINN_NS2(Data);

ShaderSerializer::ShaderSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializer(registry, outer, host) {

}

COM::Result ShaderSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == ShaderAsset::kCID);

    // ...
    auto asset = reinterpret_cast<ShaderAsset *>(object);

    // Write header
    archive << asset->shaderBlobs.Size();

    // Foreach blob
    for (auto &&blob : asset->shaderBlobs) {
        archive << static_cast<UInt32>(blob.second->stage);
        archive << blob.second->compiledBlobs.Size();

        archive << blob.second->inputAssembly.inputAttributes.Size();
        for (auto &attrib : blob.second->inputAssembly.inputAttributes) {
            archive << attrib.inputIndex;
            archive << static_cast<int>(attrib.format);
        }

        archive << blob.second->bindings.Size();
        for (auto &bind : blob.second->bindings) {
            archive << bind.id.Get();
            archive << bind.target.Get();
            archive << bind.name;
        }

        archive << blob.second->inputAssembly.inputs.Size();
        for (auto &input : blob.second->inputAssembly.inputs) {
            archive << input.stride;
            archive << static_cast<int>(input.rate);
        }

        // Write shaders
        for (auto &&it : blob.second->compiledBlobs) {
            archive << it.first;
            Check(archive << it.second);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result ShaderSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    ShaderAsset *asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}


COM::Result ShaderSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<ShaderAsset*>(object);

    // Read header
    USize blobCount;
    archive >> blobCount;

    // Foreach blob
    for (USize index = 0; index < blobCount; index++) {
        auto blob = new GHA::ShaderBlob();

        UInt32 stage;
        archive >> stage;
        blob->stage = static_cast<GHA::Stage>(stage);

        USize count;
        archive >> count;

        USize attribCount;
        archive >> attribCount;

        blob->inputAssembly.inputAttributes.Resize(attribCount);
        for (USize i = 0; i < attribCount; i++) {
            archive >> blob->inputAssembly.inputAttributes[i].inputIndex;

            int format;
            archive >> format;
            blob->inputAssembly.inputAttributes[i].format = static_cast<GHA::Format >(format);
        }

        USize bindCount;
        archive >> bindCount;

        blob->bindings.Resize(bindCount);
        for (USize i = 0 ; i < bindCount; i++) {
            USize id;
            archive >> id;
            blob->bindings[i].id = GHA::BindID(id);
            USize target;
            archive >> target;
            blob->bindings[i].target = GHA::BindID(target);
            archive >> blob->bindings[i].name;
        }

        USize inputCount;
        archive >> inputCount;

        blob->inputAssembly.inputs.Resize(inputCount);
        for (USize i = 0; i < inputCount; i++) {
            archive >> blob->inputAssembly.inputs[i].stride;
            int rate;
            archive >> rate;
            blob->inputAssembly.inputs[i].rate = static_cast<GHA::InputRate >(rate);
        }

        // Read shaders
        for (USize i = 0; i < count; i++) {
            USize classID;
            archive >> classID;

            // Read blob
            auto &compiledBlob = blob->compiledBlobs[COM::ClassID(classID)];
            Check(archive >> &compiledBlob);
        }

        // ...
        asset->shaderBlobs[blob->stage] = blob;
    }

    // OK
    return COM::kOK;
}
