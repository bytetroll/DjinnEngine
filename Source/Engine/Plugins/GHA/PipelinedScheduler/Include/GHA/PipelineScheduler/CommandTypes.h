//
// Created by Dev on 12/24/2017.
//
#pragma once

namespace Djinn::GHA::PipelineScheduler {
    enum class ResourceCommandType {
        eNone,
        eImportTexture,
        eImportVertexBuffer,
        eImportIndexBuffer,
        eImportConstantBuffer,
        eCreateSampler,
        eCreateTextureView,
        eCreateVertexBufferView,
        eCreateIndexBufferView,
        eCreateStructuredBufferView,
        eCreateLinearBufferView,
        eCreateTextureUnorderedView,
        eCreateVertexBufferUnorderedView,
        eCreateIndexBufferUnorderedView,
        eCreateStructuredBufferUnorderedView,
        eCreateLinearBufferUnorderedView,
        eCreatePipelineLayout,
        eCreatePipeline,
        eCreatePass,
        eCreateFrame,
        eCreateDescriptorSetLayout,
        eCreateDescriptorSet,
        eCreateCommandList,
        eCreateCommandPool,
        eUpdateDescriptorSet,
        eStorePersistent,
        eLoadPersistent
    };

    enum class CommandTypes {
        eNone,
        eCmdBeginPass,
        eCmdEndPass,
        eCmdCopyTexture,
        eCmdClear,
        eCmdClearColor,
        eCmdClearDepthStencil,
        eCmdBindPipeline,
        eCmdBindDescriptorSet,
        eCmdBindVertexBuffers,
        eCmdBindIndexBuffer,
        eCmdSetViewport,
        eCmdSetScissor,
        eCmdDraw,
        eCmdDrawIndexed,
        eCmdDrawIndirect,
        eCmdDrawIndirectIndexed,
        eCmdDispatch,
        eCmdDispatchIndirect,
        eCmdSignalFence,
        eCmdWaitFence,
        eCmdDebugPushEvent,
        eCmdDebugPopEvent,
        eCmdUpdateBuffer,
        eCmdUpdateDynamic,
        eCmdTextureBarrier,
    };
}