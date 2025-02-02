#include <GHA/Vulkan/DescriptorSetLayout.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

DescriptorSetLayout::DescriptorSetLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result DescriptorSetLayout::Initialize(const PipelineScheduler::DescriptorSetLayoutDesc &desc, vk::Device device) {
    this->desc = desc;
    this->device = device;

    // Collect descriptors
    vk::DescriptorSetLayoutBinding bindings[256];
    for (UInt32 i = 0; i < desc.bindings.Size(); i++) {
        bindings[i].binding = desc.bindings[i].binding;
        bindings[i].descriptorType = AsDescriptorType(desc.bindings[i].id);
        bindings[i].stageFlags = AsShaderStageFlags(desc.bindings[i].stages);
        bindings[i].descriptorCount = desc.bindings[i].count;
    }

    // Attempt to create descriptor set layout
    vk::DescriptorSetLayoutCreateInfo info;
    info.bindingCount = static_cast<UInt32>(desc.bindings.Size());
    info.pBindings = bindings;
    if (vk::CheckedResult result = device.createDescriptorSetLayout(&info, nullptr, &layout)) {
        ErrorLog(this).Write("Failed to create vulkan descriptor set layout: ", vk::to_string(result).c_str());
        return COM::kOK;
    }

    // OK
    return COM::kOK;
}

const PipelineScheduler::DescriptorSetLayoutDesc &DescriptorSetLayout::GetDesc() {
    return desc;
}

COM::Result DescriptorSetLayout::Released() {
    device.destroyDescriptorSetLayout(layout, nullptr);

    // OK
    return COM::kOK;
}
