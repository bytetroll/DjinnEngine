//
// Created by Dev on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include "DescriptorSetUpdateDesc.h"

namespace Djinn::GHA::Vulkan {
    class DescriptorAllocator;
    struct DescriptorAllocation;
    class DescriptorSetLayout;

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN DescriptorSet : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        DescriptorSet(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const DescriptorSetUpdateDesc &desc, DescriptorSetLayout *layout, DescriptorAllocator *allocator, vk::Device device);

        /// Overrides
        COM::Result Update(const DescriptorSetUpdateDesc &desc);
        const DescriptorSetUpdateDesc &GetDesc();

        /// Get descriptor set
        /// \return
        const vk::DescriptorSet &GetSet() const {
            return set;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        DescriptorSetUpdateDesc desc;
        vk::DescriptorSet set;
        DescriptorAllocation *alloc;
        DescriptorAllocator* allocator;
        vk::Device device;
    };
}