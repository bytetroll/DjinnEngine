#pragma once

#include <COM/IUnknown.h>
#include "SwapchainDesc.h"
#include "IndexBufferDesc.h"
#include "TextureBufferDesc.h"
#include "TextureDesc.h"
#include "StructuredBufferDesc.h"
#include "SamplerDesc.h"
#include "VertexBufferDesc.h"
#include "DeviceDesc.h"
#include "ShaderDesc.h"
#include "TextureBufferViewDesc.h"
#include "ConstantBufferDesc.h"

#undef CreateSemaphore

namespace Djinn {
    namespace GHA {
        class ISwapchain;
        class IIndexBuffer;
        class IVertexBuffer;
        class ITextureBuffer;
        class ITexture;
        class IStructuredBuffer;
        class IShader;
        class ISampler;
        class IAdapter;
        class IResourceStager;
        class IConstantBuffer;
        class IDeviceMemory;

        class IDevice : public COM::TUnknown<IDevice> {
        public:
            DJINN_COM_INTERFACE();

            /// Construct
            /// \param registry
            /// \param outer
            IDevice(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Get description
            /// \return
            virtual const DeviceDesc& GetDesc() = 0;

            /// Get memory
            /// \return
            virtual IDeviceMemory* GetMemory() = 0;

            /// Get adapter
            /// \return
            virtual IAdapter* GetAdapter() = 0;

            /// Get the resource stager
            /// \param queues
            /// \return
            virtual IResourceStager* GetResourceStager() = 0;

            /// Create swapchain
            /// \param desc
            /// \return
            virtual COM::Result CreateSwapchain(COM::IUnknown* outer, const SwapchainDesc& desc, ISwapchain** out) = 0;

            /// Create index buffer
            /// \param desc
            /// \return
            virtual COM::Result CreateIndexBuffer(COM::IUnknown* outer, const IndexBufferDesc& desc, IIndexBuffer** out) = 0;

            /// Create constant buffer
            /// \param desc
            /// \return
            virtual COM::Result CreateConstantBuffer(COM::IUnknown* outer, const ConstantBufferDesc& desc, IConstantBuffer** out) = 0;

            /// Create swapchain
            /// \param desc
            /// \return
            virtual COM::Result CreateSampler(COM::IUnknown* outer, const SamplerDesc& desc, ISampler** out) = 0;

            /// Create swapchain
            /// \param desc
            /// \return
            virtual COM::Result CreateShader(COM::IUnknown* outer, const ShaderDesc& desc, IShader** out) = 0;

            /// Create structured buffer
            /// \param desc
            /// \return
            virtual COM::Result CreateStructuredBuffer(COM::IUnknown* outer, const StructuredBufferDesc& desc, IStructuredBuffer** out) = 0;

            /// Create swapchain
            /// \param desc
            /// \return
            virtual COM::Result CreateTexture(COM::IUnknown* outer, const TextureDesc& desc, ITexture** out) = 0;

            /// Create swapchain
            /// \param desc
            /// \return
            virtual COM::Result CreateTextureBuffer(COM::IUnknown* outer, const TextureBufferDesc& desc, ITextureBuffer** out) = 0;

            /// Create swapchain
            /// \param desc
            /// \return
            virtual COM::Result CreateVertexBuffer(COM::IUnknown* outer, const VertexBufferDesc& desc, IVertexBuffer** out) = 0;
        };
    }
}