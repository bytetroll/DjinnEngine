#include <GHA/Vulkan/DescriptorAllocator.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <GHA/IResource.h>
#include <Core/Memory.h>
#include <GHA/Vulkan/EnumUtils.h>

DJINN_NS3(GHA, Vulkan);

struct Vulkan::DescriptorAllocationPage {
public:
    // Pool
    vk::DescriptorPool pool;

    // Remaining descriptors
    Int32 descriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];

    // All allocations
    Core::List<DescriptorAllocation *> allocations;

    // Max allocations
    UInt32 maxAllocations;
};

DescriptorAllocator::DescriptorAllocator(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result DescriptorAllocator::Initialize(vk::Device device) {
    this->device = device;


    // Helper
    auto Add = [&](vk::DescriptorType type, USize count) {
        vk::DescriptorPoolSize size;
        size.type = type;
        size.descriptorCount = static_cast<UInt32>(count);
        sizes[static_cast<int>(size.type)] = count;
        poolSizes.Add(size);
    };

    // ...
    Add(vk::DescriptorType::eCombinedImageSampler, 4096);
    Add(vk::DescriptorType::eStorageImage, 4096);
    Add(vk::DescriptorType::eSampler, 4096);
    Add(vk::DescriptorType::eUniformBuffer, 2048);
    Add(vk::DescriptorType::eStorageBuffer, 1024);
    Add(vk::DescriptorType::eStorageTexelBuffer, 1024);
    Add(vk::DescriptorType::eUniformTexelBuffer, 1024);

    // OK
    return COM::kOK;
}


COM::Result DescriptorAllocator::Allocate(const vk::DescriptorSetLayout &layout, UInt32 count, const DescriptorBinding *bindings, DescriptorAllocation **out) {
    *out = allocationPool.Pop();

    // Collect descriptor types
    Core::Memory::Zero((*out)->descriptors, sizeof(DescriptorAllocation::descriptors));
    for (UInt32 i = 0; i < count; i++) {
        (*out)->descriptors[static_cast<int>(AsDescriptorType(bindings[i].resource->GetClassID()))]++;
    }

    // Translate to descriptor types
    UInt32 bindingSizes[VK_DESCRIPTOR_TYPE_RANGE_SIZE] = {};
    for (UInt32 type = 0; type < VK_DESCRIPTOR_TYPE_RANGE_SIZE; type++) {
        bindingSizes[type] += (*out)->descriptors[type];
    }

    // Abnormal?
    bool abnormal = false;
    for (int i = 0; i < VK_DESCRIPTOR_TYPE_RANGE_SIZE; i++) {
        if (bindingSizes[i] > sizes[i]) {
            abnormal = true;
        }
    }

    // Seperate allocation for abnormal sizes
    if (abnormal) {
        // Size
        UInt32 size = 0;

        // Fill sizes
        vk::DescriptorPoolSize abnormalSizes[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
        for (int i = 0; i < VK_DESCRIPTOR_TYPE_RANGE_SIZE; i++) {
            if (bindingSizes[i] > 0) {
                abnormalSizes[size].type = static_cast<vk::DescriptorType>(i);
                abnormalSizes[size].descriptorCount = bindingSizes[i];
                size++;
            }
        }

        // Attempt to allocate pool
        vk::DescriptorPoolCreateInfo poolInfo;
        poolInfo.maxSets = 1;
        poolInfo.poolSizeCount = size;
        poolInfo.pPoolSizes = abnormalSizes;
        if (vk::CheckedResult result = device.createDescriptorPool(&poolInfo, nullptr, &(*out)->pool)) {
            ErrorLog(this).Write("Failed to allocate vulkan descriptor pool: ", vk::to_string(result).c_str());
            return COM::kError;
        }

        // Attempt set allocation
        vk::DescriptorSetAllocateInfo info;
        info.pSetLayouts = &layout;
        info.descriptorPool = (*out)->pool;
        info.descriptorSetCount = 1;
        if (vk::CheckedResult result = device.allocateDescriptorSets(&info, &(*out)->set)) {
            ErrorLog(this).Write("Failed to allocate vulkan descriptor set from expected sufficient pool: ", vk::to_string(result).c_str());
            return COM::kError;
        }

        // Diagnostic
        WarningLog(this).Write("Abnormal vulkan descriptor set allocation (Only an issue in high frequencies)");

        // ...
        return COM::kOK;
    }

    // Try current pages from back to front
    for (auto pageIt = pages.RStart(); pageIt != pages.REnd(); ++pageIt) {
        // Verify based on request
        bool ignore = false;
        for (int i = 0; i < VK_DESCRIPTOR_TYPE_RANGE_SIZE; i++) {
            if ((*pageIt)->descriptors[i] < static_cast<Int32>(bindingSizes[i])) {
                ignore = true;
                break;
            }
        }

        // Large enough?
        if (ignore || ((*pageIt)->allocations.Size() >= (*pageIt)->maxAllocations)) {
            continue;
        }

        // Attempt allocation
        vk::DescriptorSetAllocateInfo info;
        info.pSetLayouts = &layout;
        info.descriptorPool = (*pageIt)->pool;
        info.descriptorSetCount = 1;
        if (vk::CheckedResult result = device.allocateDescriptorSets(&info, &(*out)->set)) {
            ErrorLog(this).Write("Failed to allocate vulkan descriptor set from expected sufficient pool: ", vk::to_string(result).c_str());
            return COM::kError;
        }

        // ...
        (*out)->page = *pageIt;
        (*out)->pageIt = (*pageIt)->allocations.Add(*out);
    }

    // Create new descriptor page
    auto page = new DescriptorAllocationPage();

    // Attempt to allocate pool
    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = page->maxAllocations = 8192;
    poolInfo.poolSizeCount = static_cast<UInt32>(poolSizes.Size());
    poolInfo.pPoolSizes = poolSizes.Ptr();
    if (vk::CheckedResult result = device.createDescriptorPool(&poolInfo, nullptr, &page->pool)) {
        ErrorLog(this).Write("Failed to allocate vulkan descriptor pool: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // Attempt set allocation
    vk::DescriptorSetAllocateInfo info;
    info.pSetLayouts = &layout;
    info.descriptorPool = page->pool;
    info.descriptorSetCount = 1;
    if (vk::CheckedResult result = device.allocateDescriptorSets(&info, &(*out)->set)) {
        ErrorLog(this).Write("Failed to allocate vulkan descriptor set from expected sufficient pool: ", vk::to_string(result).c_str());
        return COM::kError;
    }

    // ...
    (*out)->pageIt = page->allocations.Add(*out);
    (*out)->page = page;

    // OK
    return COM::kOK;
}

void DescriptorAllocator::Free(DescriptorAllocation *alloc) {
    // Untrack
    alloc->page->allocations.Remove(alloc->pageIt);

    // Refill descriptors
    for (int i = 0; i < VK_DESCRIPTOR_TYPE_RANGE_SIZE; i++) {
        alloc->page->descriptors[i] += alloc->descriptors[i];
    }

    // Free
    allocationPool.Push(alloc);
}

COM::Result DescriptorAllocator::Released() {
    for (auto& page : pages) {
        device.destroyDescriptorPool(page->pool, nullptr);
    }

    // OK
    return COM::kOK;
}
