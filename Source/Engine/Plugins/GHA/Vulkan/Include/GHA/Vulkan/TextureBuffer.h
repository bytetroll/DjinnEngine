//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/ITextureBuffer.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA {
    class ICapabilities;
}

namespace Djinn::GHA::Vulkan {
    class Allocator;
    struct Allocation;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN TextureBuffer : public ITextureBuffer {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        TextureBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const TextureBufferDesc &desc, ICapabilities *caps, vk::Device device, Allocator *allocator);

        /// Overrides
        virtual COM::Result Map(void **out, const Core::Optional<MemoryBounds> &bounds) override;
        virtual COM::Result Unmap(void *ptr) override;
        virtual const TextureBufferDesc &GetDesc() override;
        virtual USize GetSize() override;

        /// Get buffer
        /// \return
        const vk::Buffer &GetBuffer() {
            return buffer;
        }

        /// Get view
        /// \return
        const vk::BufferView &GetView() {
            return view;
        }

        /// Get allocation
        /// \return
        Allocation *GetAllocation() const {
            return alloc;
        }

        /// Get requirements
        /// \return
        vk::MemoryRequirements GetRequirements() const {
            return requirements;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::MemoryRequirements requirements;
        TextureBufferDesc desc;
        Allocator* allocator;
        vk::Device device;
        vk::Buffer buffer;
        vk::BufferView view;
        Allocation *alloc;
    };
}