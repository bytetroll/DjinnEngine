//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/IStructuredBuffer.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class Allocator;
    struct Allocation;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN StructuredBuffer : public IStructuredBuffer {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        StructuredBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const StructuredBufferDesc &desc, vk::Device device, Allocator *allocator);

        /// Overrides
        virtual COM::Result Map(void **out, const Core::Optional<MemoryBounds> &bounds) override;
        virtual COM::Result Unmap(void *ptr) override;
        virtual const StructuredBufferDesc &GetDesc() override;
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
        Allocator* allocator;
        StructuredBufferDesc desc;
        vk::Device device;
        vk::Buffer buffer;
        vk::BufferView view;
        Allocation *alloc;
    };
}