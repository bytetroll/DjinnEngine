//
// Created by Dev on 5/13/2018.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <Core/Size.h>

namespace Djinn::GHA::Vulkan {
    /*struct AllocationCallbacks {
        AllocationCallbacks() {
            callbacks.pUserData = nullptr;
            callbacks.pfnAllocation = Allocate;
            callbacks.pfnReallocation = Reallocate;
            callbacks.pfnFree = Free;
            callbacks.pfnInternalAllocation = InternalAllocate;
            callbacks.pfnInternalFree = InternalFree;
        }

        static vk::AllocationCallbacks* Get() {
            static vk::AllocationCallbacks callbacks;
            return &callbacks;
        }

        static void* Allocate(void* user, USize size, USize alignment, VkSystemAllocationScope scope) {

        }

        static  void* Reallocate(void* user, void* original, USize size, USize alignment, VkSystemAllocationScope scope) {

        }

        static void Free(void* user, void* block) {

        }

        static void InternalAllocate(void *user, USize size, VkInternalAllocationType type, VkSystemAllocationScope scope) {

        }

        static void InternalFree(void *user, USize size, VkInternalAllocationType type, VkSystemAllocationScope scope) {

        }

    private:
        vk::AllocationCallbacks callbacks;
    };*/
}
