//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 11/23/2017.
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