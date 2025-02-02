#include <GHA/Vulkan/Allocator.h>
#include <Core/Map.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <Core/TypeCommon.h>
#include <Core/Math.h>
#include <Core/Assert.h>
#include <GHA/Vulkan/Adapter.h>
#include <Core/Literals.h>
#include <Core/Atomic.h>

DJINN_NS3(GHA, Vulkan);

struct Vulkan::AllocationPage {
public:
    // Backend memory
    vk::DeviceMemory deviceMemory;

    // Total size
    USize size;

    // Used size
    USize used;

    // Tiny page?
    bool isTiny;

    // Category of this page
    AnyHashID category;

    // All allocations
    Core::List<Allocation *> allocations;
};

struct Vulkan::AllocationHeap {
    // Backend heap
    vk::MemoryHeap heap;

    // Current offset
    USize offset = 0;
};

struct Vulkan::AllocationGroup {
    // Super heap
    AllocationHeap *heap = nullptr;

    // Currently allocated pages
    Core::Array<AllocationPage *> pages;
};

Allocator::Allocator(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IDeviceMemory(registry, outer, host) {

}

COM::Result Allocator::Initialize(GHA::IAdapter *adapter, const vk::Device device) {
    this->device = device;
    memoryProperties = static_cast<Adapter *>(adapter)->GetMemoryProperties();
    memoryProperties = static_cast<Adapter *>(adapter)->GetMemoryProperties();
    physicalDeviceProperties = static_cast<Adapter *>(adapter)->GetProperties();

    // Create heaps
    for (UInt32 i = 0; i < memoryProperties.memoryHeapCount; i++) {
        auto heap = new AllocationHeap();
        heap->heap = memoryProperties.memoryHeaps[i];
        heaps.Add(heap);
    }

    // Diagnostic
    InfoLog(this).Write(
            "Initialized, ", memoryProperties.memoryHeapCount, " available heaps with a total of ",
            physicalDeviceProperties.limits.maxMemoryAllocationCount, " available allocations"
    );

    // OK
    return COM::kOK;
}


bool Allocator::GetMemoryIndex(UInt32 bits, vk::MemoryPropertyFlags required, UInt32 &index) {
    index = Core::LimitsOf<UInt32>::kMax;
    for (UInt32 i = 0; i < memoryProperties.memoryTypeCount; i++) {
        // Valid index if:
        //  1. Supports memory type
        //  2. Supports given required flags
        //  3. Less payload than last
        if (((bits >> i) & 1)
            && (memoryProperties.memoryTypes[i].propertyFlags & required)
            && ((index == Core::LimitsOf<UInt32>::kMax) || (heaps[memoryProperties.memoryTypes[i].heapIndex]->offset < heaps[memoryProperties.memoryTypes[index].heapIndex]->offset))) {
            index = i;
        }
    }
    return index != Core::LimitsOf<UInt32>::kMax;
}

COM::Result Allocator::Allocate(const vk::MemoryRequirements &requirements, vk::MemoryPropertyFlags required, AnyHashID category, Allocation **out) {
    *out = allocationPool.Pop();

    // Determine page allocation size
    // Defaulted to 124mb
    USize pageSize = defaultPageSizes.GetOrDefault(category, 124_mb);

    // Aligned size
    USize alignedSize = Math::SnapUpper(requirements.size, requirements.alignment);

    // Get memory index
    UInt32 memoryIndex;
    if (!GetMemoryIndex(requirements.memoryTypeBits, required, memoryIndex)) {
        ErrorLog(this).Write("Failed to allocate vulkan memory, no available memory index with given required flags and memory type bits");
        return COM::kError;
    }

    // Abnormal allocation?
    // Special case where alignment requires out of bounds size
    if (alignedSize > pageSize || Math::SnapLower(pageSize - alignedSize, requirements.alignment) < alignedSize) {
        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = alignedSize;
        allocateInfo.memoryTypeIndex = memoryIndex;

        // Attempt to allocate
        vk::DeviceMemory memory;
        if (vk::CheckedResult result = device.allocateMemory(&allocateInfo, nullptr, &memory)) {
            ErrorLog(this).Write("Failed to allocate vulkan memory: << ", vk::to_string(result).c_str());
            return COM::kError;
        }

        // ...
        (*out)->page = nullptr;
        (*out)->deviceMemory = memory;
        (*out)->offset = 0;
        (*out)->size = alignedSize;
        (*out)->alignment = requirements.alignment;

        // Tracking
        totalUsed += alignedSize;
        totalAllocated += alignedSize;

        // Diagnostic
        WarningLog(this).Write("Abnormal (Only an issue in high frequencies) vulkan memory allocation  (category ", category, ") allocated with size ", alignedSize / 1e6, "mb");

        // OK
        return COM::kOK;
    }

    // Get appropriate group
    auto &group = groups[memoryIndex][category];
    if (!group) {
        group = new AllocationGroup();
        group->heap = heaps[memoryProperties.memoryTypes[memoryIndex].heapIndex];
    }

    // Distance helper
    auto SafeDist = [](USize a, USize b) { return (a > b) ? 0 : (b - a); };

    // Relatively large allocations are allocated right to left
    // TODO: Experiment with large allocation threshold
    bool isLarge = (static_cast<float>(alignedSize) / static_cast<float>(pageSize)) > 0.2f;

    // Attempt current pages
    // TODO: Testing pages from back to front, more appropriate way?
    for (auto pageIt = group->pages.RStart(); pageIt != group->pages.REnd(); ++pageIt) {
        auto page = *pageIt;

        // Must match
        if (isLarge && page->isTiny) {
            continue;
        }

        // Empty?
        if (page->allocations.Size() == 0 || SafeDist(0, page->allocations.First()->offset) >= alignedSize) {
            // Fill allocation
            (*out)->page = *pageIt;
            (*out)->size = alignedSize;
            (*out)->offset = 0;
            (*out)->alignment = requirements.alignment;
            (*out)->deviceMemory = page->deviceMemory;

            // Tracking
            totalUsed += alignedSize;

            // Insert
            if (page->allocations.Size() == 0) {
                (*out)->pageIt = page->allocations.Add(*out);
            } else {
                (*out)->pageIt = page->allocations.Add(page->allocations.Start(), *out);
            }

            // Must be aligned
            DJINN_ASSERT((*out)->offset % requirements.alignment == 0 && (*out)->size % requirements.alignment == 0, "Vulkan memory sub allocation misalignment");

            // OK
            return COM::kOK;
        } else {
            // Traverse
            Core::List<Allocation *>::Iterator end = page->allocations.End();
            for (Core::List<Allocation *>::Iterator it = page->allocations.Start(); it != end; ++it) {
                Core::List<Allocation *>::Iterator next = it;
                ++next;

                // Determine if viable
                USize offset = 0;
                if (next == end) {
                    //           |     |
                    // |it|(o---s)-----|size|
                    if ((offset = Math::SnapUpper((*it)->offset + (*it)->size, requirements.alignment)) + alignedSize > page->size) {
                        continue;
                    }
                } else {
                    //           |            |
                    // |it|(o---s)----- |next|(o---s)
                    if (SafeDist(offset = Math::SnapUpper((*it)->offset + (*it)->size, requirements.alignment), (*next)->offset) < alignedSize) {
                        continue;
                    }
                }

                // Fill allocation
                (*out)->page = *pageIt;
                (*out)->size = alignedSize;
                (*out)->offset = offset;
                (*out)->alignment = requirements.alignment;
                (*out)->deviceMemory = page->deviceMemory;

                // Insert
                (*out)->pageIt = page->allocations.Add(next, *out);

                // Tracking
                page->used += alignedSize;
                totalUsed += alignedSize;

                // Must be aligned
                DJINN_ASSERT((*out)->offset % requirements.alignment == 0 && (*out)->size % requirements.alignment == 0, "Vulkan memory sub allocation misalignment");

                // OK
                return COM::kOK;
            }
        }
    }

    // No page available, create new page
    auto page = new AllocationPage();
    page->size = pageSize;
    page->category = category;
    page->isTiny = !isLarge;
    page->isTiny = !isLarge;
    page->used = 0;
    group->pages.Add(page);
    group->heap->offset += pageSize;

    // Diagnostic
    InfoLog(this).Write("Page (category ", category, ") allocated with size ", pageSize / 1e6, "mb");

    // Allocate page memory
    vk::MemoryAllocateInfo allocateInfo;
    allocateInfo.memoryTypeIndex = memoryIndex;
    allocateInfo.allocationSize = pageSize;
    if (vk::CheckedResult result = device.allocateMemory(&allocateInfo, nullptr, &page->deviceMemory)) {
        ErrorLog(this).Write("Failed to allocate vulkan memory: << ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Create sub allocation
    (*out)->deviceMemory = page->deviceMemory;
    (*out)->page = page;
    (*out)->offset = 0;
    (*out)->size = alignedSize;
    (*out)->alignment = requirements.alignment;
    page->used += alignedSize;
    page->allocations.Add(*out);
    (*out)->pageIt = page->allocations.Start();

    // Tracking
    totalUsed += alignedSize;
    totalAllocated += page->size;

    // OK
    return COM::kOK;
}

void Allocator::Free(Allocation *alloc) {
    // Tracking
    totalUsed -= alloc->size;

    // May be free allocation
    if (alloc->page) {
        // Remove allocation
        alloc->page->allocations.Remove(alloc->pageIt);

        // Tracking
        alloc->page->used -= alloc->size;
    } else {
        // Tracking
        totalAllocated -= alloc->size;

        // Release abnormal allocation
        device.freeMemory(alloc->deviceMemory, nullptr);
    }

    // ...
    allocationPool.Push(alloc);
}

void Allocator::SetCategoryPageSize(UInt64 category, USize size) {
    defaultPageSizes[category] = size;
}

USize Allocator::GetAllocatedMemory() {
    return static_cast<USize>(totalAllocated.Load());
}

USize Allocator::GetUsedMemory() {
    return static_cast<USize>(totalUsed.Load());
}

COM::Result Allocator::Released() {
    // Free all pages
    for (auto kv : groups) {
        for (auto group : kv.second) {
            for (auto page : group.second->pages) {
                // Report live allocations
                if (page->allocations.Size() > 0) {
                    ErrorLog(this).Write("Allocated heap page with category '", page->category.GetString(), "' still has ", page->used, " bytes used with ", page->allocations.Size(), " live allocations");
                }

                // ...
                device.freeMemory(page->deviceMemory, nullptr);
            }
        }
    }

    // OK
    return COM::kOK;
}
