#include <GHA/Vulkan/Pass.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

Pass::Pass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}


const PipelineScheduler::PassDesc &Pass::GetDesc() {
    return desc;
}

COM::Result Pass::Initialize(const PipelineScheduler::PassDesc &desc, vk::Device device) {
    this->desc = desc;
    this->device = device;

    /*
     * Color Targets
     * Depth Target
     * Color Resolve Targets
     * Depth Resolve Target
     * */

    // Collect dependencies
    vk::SubpassDependency dependencies[16];
    for (UInt32 i = 0; i < desc.dependencyCount; i++) {
        dependencies[i].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[i].dstSubpass = 0;
        dependencies[i].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
        dependencies[i].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependencies[i].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
        dependencies[i].dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        dependencies[i].dependencyFlags = vk::DependencyFlagBits::eByRegion;
    }

    Core::FixedArray32<vk::AttachmentDescription> attachments;
    Core::FixedArray12<vk::AttachmentReference> resolveReferences;

    // Collect attachments
    for (UInt32 i = 0; i < desc.colorRenderTargets.Size(); i++) {
        vk::AttachmentDescription attachment;
        attachment.format = AsFormat(desc.colorRenderTargets[i].format);
        attachment.loadOp = AsAttachmentLoadOp(desc.colorRenderTargets[i].colorLoadOp);
        attachment.storeOp = AsAttachmentStoreOp(desc.colorRenderTargets[i].colorStoreOp);
        attachment.stencilLoadOp = AsAttachmentLoadOp(desc.colorRenderTargets[i].stencilLoadOp);
        attachment.stencilStoreOp = AsAttachmentStoreOp(desc.colorRenderTargets[i].stencilStoreOp);
        attachment.initialLayout = AsImageLayout(desc.colorRenderTargets[i].initialLayout);
        attachment.finalLayout = AsImageLayout(desc.colorRenderTargets[i].finalLayout);
        attachment.samples = AsSampleCountFlag(desc.colorRenderTargets[i].sampleMode);
        attachments.Add(attachment);
    }

    // Append depth stencil
    if (desc.depthStencilTarget.IsValid()) {
        vk::AttachmentDescription attachment;
        attachment.format = AsFormat(desc.depthStencilTarget->format);
        attachment.loadOp = AsAttachmentLoadOp(desc.depthStencilTarget->colorLoadOp);
        attachment.storeOp = AsAttachmentStoreOp(desc.depthStencilTarget->colorStoreOp);
        attachment.stencilLoadOp = AsAttachmentLoadOp(desc.depthStencilTarget->stencilLoadOp);
        attachment.stencilStoreOp = AsAttachmentStoreOp(desc.depthStencilTarget->stencilStoreOp);
        attachment.initialLayout = AsImageLayout(desc.depthStencilTarget->initialLayout);
        attachment.finalLayout = AsImageLayout(desc.depthStencilTarget->finalLayout);
        attachment.samples = AsSampleCountFlag(desc.depthStencilTarget->sampleMode);
        attachments.Add(attachment);
    }

    // Collect attachments
    for (UInt32 i = 0; i < desc.colorResolveRenderTargets.Size(); i++) {
            vk::AttachmentDescription attachment;
            attachment.format = AsFormat(desc.colorResolveRenderTargets[i].format);
            attachment.loadOp = AsAttachmentLoadOp(desc.colorResolveRenderTargets[i].colorLoadOp);
            attachment.storeOp = AsAttachmentStoreOp(desc.colorResolveRenderTargets[i].colorStoreOp);
            attachment.stencilLoadOp = AsAttachmentLoadOp(desc.colorResolveRenderTargets[i].stencilLoadOp);
            attachment.stencilStoreOp = AsAttachmentStoreOp(desc.colorResolveRenderTargets[i].stencilStoreOp);
            attachment.initialLayout = AsImageLayout(desc.colorResolveRenderTargets[i].initialLayout);
            attachment.finalLayout = AsImageLayout(desc.colorResolveRenderTargets[i].finalLayout);
            attachment.samples = vk::SampleCountFlagBits ::e1;
            attachments.Add(attachment);

            vk::AttachmentReference reference;
            reference.attachment = static_cast<UInt32>(attachments.Size() - 1);
            reference.layout = AsImageLayout(desc.colorResolveRenderTargets[i].initialLayout);
            resolveReferences.Add(reference);
    }

    // Collect attachments
    if (desc.depthResolveStencilTarget.IsValid()) {
        vk::AttachmentDescription attachment;
        attachment.format = AsFormat(desc.depthResolveStencilTarget->format);
        attachment.loadOp = AsAttachmentLoadOp(desc.depthResolveStencilTarget->colorLoadOp);
        attachment.storeOp = AsAttachmentStoreOp(desc.depthResolveStencilTarget->colorStoreOp);
        attachment.stencilLoadOp = AsAttachmentLoadOp(desc.depthResolveStencilTarget->stencilLoadOp);
        attachment.stencilStoreOp = AsAttachmentStoreOp(desc.depthResolveStencilTarget->stencilStoreOp);
        attachment.initialLayout = AsImageLayout(desc.depthResolveStencilTarget->initialLayout);
        attachment.finalLayout = AsImageLayout(desc.depthResolveStencilTarget->finalLayout);
        attachment.samples = vk::SampleCountFlagBits ::e1;
        attachments.Add(attachment);

        vk::AttachmentReference reference;
        reference.attachment = static_cast<UInt32>(attachments.Size() - 1);
        reference.layout = AsImageLayout(desc.depthResolveStencilTarget->initialLayout);
        resolveReferences.Add(reference);
    }

    // Collect references
    vk::AttachmentReference references[16];
    for (UInt32 i = 0; i < desc.colorRenderTargets.Size(); i++) {
        references[i].attachment = i;
        references[i].layout = AsImageLayout(desc.colorRenderTargets[i].initialLayout);
    }

    // Append depth stencil
    if (desc.depthStencilTarget.IsValid()) {
        references[desc.colorRenderTargets.Size()].attachment = static_cast<UInt32>(desc.colorRenderTargets.Size());
        references[desc.colorRenderTargets.Size()].layout = AsImageLayout(desc.depthStencilTarget->initialLayout);
    }

    // Describe subpass
    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    if (resolveReferences.Size() > 0) {
        subpass.pResolveAttachments = resolveReferences.Ptr();
    }
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = 0;
    subpass.colorAttachmentCount = static_cast<UInt32>(desc.colorRenderTargets.Size());
    subpass.pColorAttachments = references;
    if (desc.depthStencilTarget.IsValid()) {
        subpass.pDepthStencilAttachment = &references[desc.colorRenderTargets.Size()];
    }

    // Attempt to create pass
    vk::RenderPassCreateInfo info;
    info.dependencyCount = desc.dependencyCount;
    info.pDependencies = dependencies;
    info.attachmentCount = static_cast<UInt32>(attachments.Size());
    info.pAttachments = attachments.Ptr();
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    if (vk::CheckedResult result = device.createRenderPass(&info, nullptr, &pass)) {
        ErrorLog(this).Write("Failed to create vulkan render pass ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result Pass::Released() {
    device.destroyRenderPass(pass, nullptr);

    // OK
    return COM::kOK;
}
