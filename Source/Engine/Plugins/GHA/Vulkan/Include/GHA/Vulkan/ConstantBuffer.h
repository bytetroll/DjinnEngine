//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/IConstantBuffer.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class Allocator;
    struct Allocation;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN ConstantBuffer : public IConstantBuffer {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        ConstantBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const ConstantBufferDesc &desc, vk::Device device, Allocator *allocator);

        /// Overrides
        virtual COM::Result Map(void **out, const Core::Optional<MemoryBounds> &bounds) override;
        virtual COM::Result Unmap(void *ptr) override;
        const ConstantBufferDesc &GetDesc() override;
        virtual USize GetSize() override;

        /// Get buffer
        /// \return
        const vk::Buffer &GetBuffer() const {
            return buffer;
        }

        /// Get descriptor
        /// \return
        const vk::DescriptorBufferInfo &GetDescriptor() {
            return descriptor;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        Allocation *alloc;
        vk::Buffer buffer;
        vk::Device device;
        vk::MemoryRequirements requirements;
        Allocator *allocator;
        ConstantBufferDesc desc;
        vk::DescriptorBufferInfo descriptor;
    };
}