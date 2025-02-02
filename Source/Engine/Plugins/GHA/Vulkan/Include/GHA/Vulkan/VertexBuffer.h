//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <GHA/IVertexBuffer.h>
#include <vulkan/vulkan.ext.hpp>

namespace Djinn::GHA::Vulkan {
    class Allocator;
    struct Allocation;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN VertexBuffer : public IVertexBuffer {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        VertexBuffer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const VertexBufferDesc &desc, vk::Device device, Allocator *allocator);

        /// Overrides
        virtual COM::Result Map(void **out, const Core::Optional<MemoryBounds> &bounds) override;
        virtual COM::Result Unmap(void *ptr) override;
        virtual const VertexBufferDesc &GetDesc() override;
        virtual USize GetSize() override;

        /// Get buffer
        /// \return
        const vk::Buffer &GetBuffer() const {
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
        vk::Buffer buffer;
        VertexBufferDesc desc;
        Allocator* allocator;
        vk::BufferView view;
        vk::Device device;
        Allocation *alloc;
    };
}