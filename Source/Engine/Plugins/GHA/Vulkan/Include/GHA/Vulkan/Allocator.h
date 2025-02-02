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

#include <COM/IUnknown.h>
#include <Core/String.h>
#include <vulkan/vulkan.ext.hpp>
#include <Core/Pool.h>
#include <Core/HashMap.h>
#include <Core/List.h>
#include <Core/Array.h>
#include <GHA/IDeviceMemory.h>
#include <Core/Atomic.h>
#include <Core/Set.h>

namespace Djinn::GHA {
    class IAdapter;
}

namespace Djinn::GHA::Vulkan {
    struct AllocationPage;
    struct AllocationHeap;
    struct AllocationGroup;

    /// A single allocation
    struct Allocation {
    public:
        AllocationPage *page;
        Core::List<Allocation *>::Iterator pageIt;
        vk::DeviceMemory deviceMemory;
        USize size;
        USize offset;
        USize alignment;
    };

    class DJINN_EXPORT_PLUGINS_GHA_VULKAN Allocator : public IDeviceMemory {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Allocator(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(GHA::IAdapter *adapter, const vk::Device device);

        /// Set the desired default heap size of given category
        /// \param size
        void SetCategoryPageSize(UInt64 category, USize size);

        /// Allocate based on requirements
        /// \param requirements
        /// \param category - All other allocations with the same category will share the same heap
        /// \return
        COM::Result Allocate(const vk::MemoryRequirements &requirements, vk::MemoryPropertyFlags required, AnyHashID category, Allocation **out);

        /// Free an allocation, note alloc is invalidated
        /// \param alloc
        void Free(Allocation *alloc);

        /// Get optimal memory index from bits
        /// \param bits
        /// \param required - Extra required flags
        /// \return
        bool GetMemoryIndex(UInt32 bits, vk::MemoryPropertyFlags required, UInt32 &index);

        /// Overrides
        virtual USize GetAllocatedMemory() override;
        virtual USize GetUsedMemory() override;

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        vk::Device device;

        // All available heaps
        Core::Array<AllocationHeap *> heaps;

        // All allocation groups (Memory Bits -> Category -> Pages>
        Core::HashMap<UInt32, Core::HashMap<UInt64, AllocationGroup *>> groups;

        // Category heap size hints
        Core::HashMap<UInt64, USize> defaultPageSizes;

        // Allocation pool
        Core::ObjectPool<Allocation> allocationPool;

        // Memory properties
        vk::PhysicalDeviceMemoryProperties memoryProperties;

        // Capabilities
        vk::PhysicalDeviceProperties physicalDeviceProperties;

        // Total allocated space
        Core::AtomicCounter totalAllocated{0};

        // Total used space
        Core::AtomicCounter totalUsed{0};
    };
}