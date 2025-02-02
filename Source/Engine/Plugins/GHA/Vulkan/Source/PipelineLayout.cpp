#include <GHA/Vulkan/PipelineLayout.h>
#include <GHA/Vulkan/DescriptorSetLayout.h>
#include <Core/Assert.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

PipelineLayout::PipelineLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}

COM::Result PipelineLayout::Initialize(const PipelineLayoutDesc &desc, vk::Device device) {
    this->desc = desc;
    this->device = device;

    // Collect sets
    vk::DescriptorSetLayout setLayouts[32];
    for (USize i = 0; i < desc.layouts.Size(); i++) {
        setLayouts[i] = desc.layouts[i]->GetLayout();
    }

    // Collect ranges
    vk::PushConstantRange ranges[32];
    for (USize i = 0; i < desc.pushConstants.Size(); i++) {
        ranges[i].stageFlags = AsShaderStageFlags(desc.pushConstants[i].stage);
        ranges[i].size = desc.pushConstants[i].size;
        ranges[i].offset = desc.pushConstants[i].offset;
    }

    // Attempt to create pipeline layout
    vk::PipelineLayoutCreateInfo info;
    info.setLayoutCount = static_cast<UInt32>(desc.layouts.Size());
    info.pSetLayouts = setLayouts;
    info.pushConstantRangeCount = static_cast<UInt32>(desc.pushConstants.Size());
    info.pPushConstantRanges = ranges;
    if (vk::CheckedResult result = device.createPipelineLayout(&info, nullptr, &pipelineLayout)) {
        ErrorLog(this).Write("Failed to create vulkan pipeline layout: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

const PipelineLayoutDesc &PipelineLayout::GetDesc() {
    return desc;
}

COM::Result PipelineLayout::Released() {
    device.destroyPipelineLayout(pipelineLayout, nullptr);

    // OK
    return COM::kOK;
}
