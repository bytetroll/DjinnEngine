//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <Core/List.h>
#include <Core/Pool.h>
#include <Core/Array.h>
#include "DescriptorBinding.h"

namespace Djinn::GHA::Vulkan {
    struct DescriptorAllocationPage;

    /// Represents a single descriptor set allocation
    struct DescriptorAllocation {
        DescriptorAllocationPage *page;
        Core::List<DescriptorAllocation *>::Iterator pageIt;
        Int32 descriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
        vk::DescriptorSet set;
        vk::DescriptorPool pool;
    };

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN DescriptorAllocator : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        DescriptorAllocator(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(vk::Device device);

        /// Attempt to allocate new descriptor set
        /// \param info
        /// \param out
        /// \return
        COM::Result Allocate(const vk::DescriptorSetLayout &layout, UInt32 count, const DescriptorBinding *bindings, DescriptorAllocation **out);

        /// Free an allocation, will invalidate
        /// \param alloc
        void Free(DescriptorAllocation *alloc);

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Device device;
        vk::DescriptorPool buffer;

        // Default pool sizes
        vk::DeviceSize sizes[static_cast<int>(vk::DescriptorType::eInputAttachment) + 1] = {};
        Core::Array<vk::DescriptorPoolSize> poolSizes;

        // Pages
        Core::Array<DescriptorAllocationPage *> pages;

        // Allocation pool
        Core::ObjectPool<DescriptorAllocation> allocationPool;
    };
}