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
 on 5/13/2018.
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
