//
// Created by Dev on 1/31/2018.
//

#include <GHA/Vulkan/CompiledShaderBlobSerializer.h>
#include <GHA/Vulkan/CompiledShaderBlob.h>
#include <Host/IRegistry.h>
#include <Serialization/Common/Containers.h>

DJINN_NS3(GHA, Vulkan);

CompiledShaderBlobSerializer::CompiledShaderBlobSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializer(registry, outer, host) {

}

COM::Result CompiledShaderBlobSerializer::Serialize(Serialization::Archive &archive, COM::IUnknown *object) {
    DJINN_ASSERT(object->GetClassID() == CompiledShaderBlob::kCID);

    // ...
    auto blob = static_cast<CompiledShaderBlob*>(object);

    // Write
    archive << blob->blob.Size();
    archive.GetStream()->Write(blob->blob.Ptr(), blob->blob.ByteSize());

    // Write
    archive << blob->dynamicSize;
    archive << blob->dynamicOffset;

    // Write offsets
    archive << blob->dynamicBindingOffsets.Size();
    for (const auto& kv : blob->dynamicBindingOffsets) {
        archive << kv.first;
        archive << kv.second;
    }

    // Write header
    archive << blob->resources.Size();
    for (const auto& kv : blob->resources) {
        archive << kv.second.Size();

        for (auto& resource : kv.second) {
            archive << resource.name;
            archive << resource.index;
            archive << resource.set;
            archive << resource.count;
            archive << static_cast<int>(resource.rid);
        }
    }

    // OK
    return COM::kOK;
}

COM::Result CompiledShaderBlobSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown **object) {
    CompiledShaderBlob* asset;
    if (!registry->CreateClass(nullptr, &asset)) {
        return COM::kError;
    }

    if (COM::CheckedResult result = Deserialize(archive, asset)) {
        return result;
    }

    *object = asset;
    return COM::kOK;
}


COM::Result CompiledShaderBlobSerializer::Deserialize(Serialization::Archive &archive, COM::IUnknown *object) {
    auto asset = static_cast<CompiledShaderBlob*>(object);

    // Read header
    USize size;
    archive >> size;

    // Read blob
    asset->blob.Resize(size);
    archive.GetStream()->Read(asset->blob.Ptr(), asset->blob.ByteSize());

    // Read
    archive >> asset->dynamicSize;
    archive >> asset->dynamicOffset;

    // Read header
    USize dynamicCount;
    archive >> dynamicCount;

    // Read resources
    for (USize i = 0; i < dynamicCount; i++) {
        USize id;
        archive >> id;
        UInt32 offset;
        archive >> offset;
        asset->dynamicBindingOffsets[id] = offset;
    }

    // Read header
    USize resourceCount;
    archive >> resourceCount;

    // Read resources
    for (USize i = 0; i < resourceCount; i++) {
        USize count;
        archive >> count;

        for (USize j = 0; j < count; j++) {
            PipelineScheduler::CompiledDescriptor descriptor;
            archive >> descriptor.name;
            archive >> descriptor.index;
            archive >> descriptor.set;
            archive >> descriptor.count;
            int rid;
            archive >> rid;
            descriptor.rid = static_cast<PipelineScheduler::DescriptorType>(rid);
            asset->resources[descriptor.set].Add(descriptor);
        }
    }

    // OK
    return COM::kOK;
}
