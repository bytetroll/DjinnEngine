#include <GHA/Vulkan/Pipeline.h>
#include <GHA/Vulkan/Shader.h>
#include <GHA/Vulkan/Pass.h>
#include <GHA/Vulkan/Pass.h>
#include <Core/Assert.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Core/Memory.h>
#include <GHA/Vulkan/PipelineLayout.h>
#include <GHA/Vulkan/Pass.h>
#include <GHA/InputAssemblyDesc.h>
#include <GHA/ICapabilities.h>
#include <GHA/Vulkan/EnumUtils.h>
#include <GHA/ShaderBlob.h>
#include <Core/Timer.h>

DJINN_NS3(GHA, Vulkan);

Pipeline::Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

}

COM::Result Pipeline::Initialize(const PipelineScheduler::PipelineDesc &desc, PipelineLayout *layout, Pass *pass, vk::PipelineCache &cache, GHA::ICapabilities *caps, vk::Device device) {
    this->desc = desc;
    this->device = device;

    // Diagnostic
    Core::Timer timer;

    // Must have one shader
    DJINN_ASSERT(desc.shaders.Size() > 0, "Pipeline must have at least one shader");

    // Shader mask
    GHA::StageSet stages;

    // Collect shaders
    vk::PipelineShaderStageCreateInfo shaders[16];
    for (UInt32 i = 0; i < desc.shaders.Size(); i++) {
        auto stage = desc.shaders[i]->GetDesc().blob->stage;

        // Stage must not be duplicated
        DJINN_ASSERT((stages & stage) == 0, "Duplicated stage");

        // Fill
        shaders[i].pName = "main";
        shaders[i].stage = AsShaderStageFlag(stage);
        shaders[i].module = static_cast<Shader *>(desc.shaders[i])->GetModule();
        stages += stage;
    }

    // Compute?
    if (stages & GHA::Stage::eCompute) {
        bindPoint = GHA::BindPoint::eCompute;

        // Compute stage is exclusive
        DJINN_ASSERT(stages == GHA::Stage::eCompute, "Compute shaders are exclusive in pipelines");

        // Attempt to create compute pipeline
        vk::ComputePipelineCreateInfo info;
        info.layout = layout->GetLayout();
        info.stage = shaders[0];
        if (vk::CheckedResult result = device.createComputePipelines(cache, 1, &info, nullptr, &pipeline)) {
            ErrorLog(this).Write("Failed to create vulkan graphics pipeline: ", vk::to_string(result).c_str());
            return COM::kError;
        }
    } else {
        bindPoint = GHA::BindPoint::eGraphics;

        // ...
        DJINN_ASSERT(pass != nullptr, "Graphics pipelines must have an attached pass");

        // Default sample mask
        UInt32 sampleMask = 0xFFFFFFFF;

        // Describe multisampling state
        vk::PipelineMultisampleStateCreateInfo msInfo;
        msInfo.alphaToCoverageEnable = static_cast<vk::Bool32>(desc.multiSample.alphaCoverage);
        msInfo.alphaToOneEnable = static_cast<vk::Bool32>( desc.multiSample.alphaOne);
        msInfo.minSampleShading = 0;
        msInfo.rasterizationSamples = AsSampleCountFlag(desc.multiSample.sampleMode);
        msInfo.pSampleMask = &sampleMask;

        // Describe rasterizer state
        vk::PipelineRasterizationStateCreateInfo rsInfo;
        rsInfo.cullMode = AsCullModeFlag(desc.rasterizer.cullMode);
        rsInfo.depthBiasClamp = 0.0f;
        rsInfo.depthBiasConstantFactor = 0.0f;
        rsInfo.depthClampEnable = static_cast<vk::Bool32>(false);
        rsInfo.rasterizerDiscardEnable = static_cast<vk::Bool32>(false);
        rsInfo.polygonMode = AsPolygonMode(desc.rasterizer.fillMode);
        rsInfo.frontFace = (desc.rasterizer.isCounterClockwise ? vk::FrontFace::eCounterClockwise : vk::FrontFace::eClockwise);
        rsInfo.depthBiasEnable = static_cast<vk::Bool32>(false);
        rsInfo.depthBiasSlopeFactor = 0;
        rsInfo.lineWidth = desc.rasterizer.lineWidth;

        // Describe blend states per target
        vk::PipelineColorBlendAttachmentState bsAttachmentInfos[16];
        for (USize i = 0; i < desc.blend.renderTargets.Size(); i++) {
            bsAttachmentInfos[i].blendEnable = static_cast<vk::Bool32>(desc.blend.renderTargets[i].enabled);
            bsAttachmentInfos[i].alphaBlendOp = AsBlendOp(desc.blend.renderTargets[i].alphaOp);
            bsAttachmentInfos[i].colorBlendOp = AsBlendOp(desc.blend.renderTargets[i].colorOp);
            bsAttachmentInfos[i].srcAlphaBlendFactor = AsBlendFactor(desc.blend.renderTargets[i].alphaSrcFactor);
            bsAttachmentInfos[i].dstAlphaBlendFactor = AsBlendFactor(desc.blend.renderTargets[i].alphaDstFactor);
            bsAttachmentInfos[i].srcColorBlendFactor = AsBlendFactor(desc.blend.renderTargets[i].colorSrcFactor);
            bsAttachmentInfos[i].dstColorBlendFactor = AsBlendFactor(desc.blend.renderTargets[i].colorDstFactor);
            bsAttachmentInfos[i].colorWriteMask = AsColorComponentFlags(desc.blend.renderTargets[i].mask);
        }

        // Describe blend state
        vk::PipelineColorBlendStateCreateInfo bsInfo;
        Core::Memory::Copy(bsInfo.blendConstants, desc.blend.coefficients.Ptr(), sizeof(bsInfo.blendConstants));
        bsInfo.logicOp = vk::LogicOp::eNoOp;
        bsInfo.logicOpEnable = static_cast<vk::Bool32>( false);
        bsInfo.attachmentCount = static_cast<Int32>(desc.blend.renderTargets.Size());
        bsInfo.pAttachments = bsAttachmentInfos;

        // Default dynamic states
        vk::DynamicState dynamicStates[] = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor,
        };

        // Describe dynamic states
        vk::PipelineDynamicStateCreateInfo dynInfo;
        dynInfo.dynamicStateCount = sizeof(dynamicStates) / sizeof(vk::DynamicState);
        dynInfo.pDynamicStates = dynamicStates;

        // Describe depth stencil state
        vk::PipelineDepthStencilStateCreateInfo dsInfo;
        dsInfo.depthBoundsTestEnable = static_cast<vk::Bool32>(true);
        dsInfo.depthCompareOp = AsCompareOp(desc.depthStencil.depthOp);
        dsInfo.depthTestEnable = static_cast<vk::Bool32>(desc.depthStencil.depthRead);
        dsInfo.depthWriteEnable = static_cast<vk::Bool32>(desc.depthStencil.depthWrite);
        dsInfo.minDepthBounds = desc.depthStencil.minDepth;
        dsInfo.maxDepthBounds = desc.depthStencil.maxDepth;
        dsInfo.stencilTestEnable = static_cast<vk::Bool32>(desc.depthStencil.stencilRead);
        dsInfo.front.failOp = vk::StencilOp::eZero;
        dsInfo.front.passOp = vk::StencilOp::eKeep;
        dsInfo.front.depthFailOp = vk::StencilOp::eZero;
        dsInfo.front.compareOp = vk::CompareOp::eAlways;
        dsInfo.front.compareMask = 0;
        dsInfo.front.writeMask = 0;
        dsInfo.front.reference = 0;
        dsInfo.back = dsInfo.front;

        // Describe tesselation state
        //vk::PipelineTessellationStateCreateInfo tsInfo;
        //tsInfo.patchControlPoints

        // Describe vertex attributes
        vk::VertexInputAttributeDescription vertexAttributes[32];
        UInt32 offsets[12] = {};
        for (USize i = 0; i < desc.inputAssembly.inputAttributes.Size(); i++) {
            vertexAttributes[i].format = AsFormat(desc.inputAssembly.inputAttributes[i].format);
            vertexAttributes[i].binding = desc.inputAssembly.inputAttributes[i].inputIndex;
            vertexAttributes[i].location = static_cast<UInt32>(i);
            vertexAttributes[i].offset = static_cast<UInt32>(offsets[desc.inputAssembly.inputAttributes[i].inputIndex]);
            offsets[desc.inputAssembly.inputAttributes[i].inputIndex] += caps->GetFormatSize(desc.inputAssembly.inputAttributes[i].format);
        }

        // Describe vertex bindings
        vk::VertexInputBindingDescription vertexBindings[32];
        for (USize i = 0; i < desc.inputAssembly.inputs.Size(); i++) {
            vertexBindings[i].stride = desc.inputAssembly.inputs[i].stride;
            vertexBindings[i].inputRate = AsInputRate(desc.inputAssembly.inputs[i].rate);
            vertexBindings[i].binding = static_cast<UInt32>(i);
        }

        // Describe vertex input state
        vk::PipelineVertexInputStateCreateInfo viInfo;
        viInfo.vertexAttributeDescriptionCount = static_cast<UInt32>(desc.inputAssembly.inputAttributes.Size());
        viInfo.pVertexAttributeDescriptions = vertexAttributes;
        viInfo.vertexBindingDescriptionCount = static_cast<UInt32>(desc.inputAssembly.inputs.Size());
        viInfo.pVertexBindingDescriptions = vertexBindings;

        // Describe input assembly state
        vk::PipelineInputAssemblyStateCreateInfo iaInfo;
        iaInfo.topology = AsTopology(desc.topology);
        iaInfo.primitiveRestartEnable = static_cast<vk::Bool32>(false);

        // Viewport state
        vk::PipelineViewportStateCreateInfo vpInfo;
        //vpInfo.scissorCount = desc.viewportCount;
        //vpInfo.viewportCount = desc.viewportCount;
        vpInfo.scissorCount = 1;
        vpInfo.viewportCount = 1;

        // Attempt to create graphics pipeline
        vk::GraphicsPipelineCreateInfo info;
        info.layout = layout->GetLayout();
        info.stageCount = static_cast<UInt32>(desc.shaders.Size());
        info.renderPass = pass->GetPass();
        info.pStages = shaders;
        info.pViewportState = &vpInfo;
        info.pDepthStencilState = &dsInfo;
        info.pMultisampleState = &msInfo;
        info.pDynamicState = &dynInfo;
        info.pColorBlendState = &bsInfo;
        info.pRasterizationState = &rsInfo;
        //info.pTessellationState = &tsInfo;
        info.pVertexInputState = &viInfo;
        info.pInputAssemblyState = &iaInfo;
        if (vk::CheckedResult result = device.createGraphicsPipelines(cache, 1, &info, nullptr, &pipeline)) {
            ErrorLog(this).Write("Failed to create vulkan compute pipeline: ", vk::to_string(result).c_str());
            return COM::kError;
        }
    }

    // Diagnostic
    InfoLog(this).Write("Created vulkan pipeline ", timer.Delta() * 1000, "ms");

    // OK
    return COM::kOK;
}

PipelineScheduler::PipelineBinding Pipeline::GetBind(UInt64 hashCrc64, GHA::StageSet stageMask) {
    return {};
}

const PipelineScheduler::PipelineDesc &Pipeline::GetDesc() {
    return desc;
}

GHA::BindPoint Pipeline::GetBindPoint() {
    return bindPoint;
}

COM::Result Pipeline::Released() {
    device.destroyPipeline(pipeline, nullptr);

    // OK
    return COM::kOK;
}
