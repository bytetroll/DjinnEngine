#include <GHA/Vulkan/Frame.h>
#include <GHA/Vulkan/Pass.h>
#include <GHA/Vulkan/TextureView.h>
#include <GHA/Vulkan/Texture.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/PipelineScheduler/FrameDesc.h>

DJINN_NS3(GHA, Vulkan);

Frame::Frame(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}

COM::Result Frame::Initialize(const FrameDesc &desc, Pass *pass, vk::Device device) {
    this->desc = desc;
    this->device = device;

    // ...
    auto vkPass = static_cast<Pass *>(pass);

    // Collect attachment views
    Core::FixedArray32<vk::ImageView> attachments;
    for (UInt32 i = 0; i < desc.renderTargets.Size(); i++) {
        attachments.Add(desc.renderTargets[i]->GetView());
    }

    for (UInt32 i = 0; i < desc.resolveRenderTargets.Size(); i++) {
        attachments.Add(desc.resolveRenderTargets[i]->GetView());
    }

    // Attempt to create frame buffer
    vk::FramebufferCreateInfo info;
    info.width = desc.renderArea.extent.width;
    info.height = desc.renderArea.extent.height;
    info.layers = desc.renderArea.extent.depth;
    info.attachmentCount = static_cast<UInt32>(attachments.Size());
    info.pAttachments = attachments.Ptr();
    info.renderPass = vkPass->GetPass();
    if (vk::CheckedResult result = device.createFramebuffer(&info, nullptr, &frameBuffer)) {
        ErrorLog(this).Write("Failed to create vulkan frame buffer: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

const FrameDesc &Frame::GetDesc() {
    return desc;
}

COM::Result Frame::Released() {
    device.destroyFramebuffer(frameBuffer, nullptr);

    // OK
    return COM::kOK;
}
