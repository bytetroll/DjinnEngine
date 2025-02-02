//
// Created by Dev on 12/22/2017.
//
#pragma once

namespace Djinn::GHA {
    /// Resource command types
    enum class ResourceCommandType {
        eNone,
        eAddTexture,
        eImportVertexBuffer,
        eImportIndexBuffer,
        eImportConstantBuffer,
        eImportTexture,
        eImportTextureStreamable,
        eImportVertexStreamable,
        eImportIndexStreamable,
    };

    /// Abstract command types
    enum class CommandType {
        eNone,
        eDebugPushEvent,
        eDebugPopEvent,
        eCmdCopyTexture,
        eCmdClear,
        eCmdClearColor,
        eCmdClearDepthStencil,
        eSetTopology,
        eBindRenderTarget,
        eBindDepthRenderTarget,
        eBindResource,
        eBindTexture,
        eBindUnorderedTextures,
        eBindVertexBuffer,
        eBindIndexBuffer,
        eSetRasterizerState,
        eSetMultisampleState,
        eSetDepthStencilState,
        eSetViewport,
        eSetScissor,
        eBindShader,
        eBindShaderBlob,
        eBindPacked,
        eBindDynamic,
        eSetSampleMode,
        eDraw,
        eDrawIndexed,
        eDrawIndirect,
        eDrawIndirectIndexed,
        eDispatch,
        eDispatchIndirect,
        eSignalFence,
        eWaitFence,
    };
}