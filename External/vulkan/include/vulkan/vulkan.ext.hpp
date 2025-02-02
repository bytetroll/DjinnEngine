//
// Created by Dev on 11/23/2017.
//
#pragma once

#ifdef DJINN_PLATFORM_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#else
#   define VK_USE_PLATFORM_XLIB_KHR
#endif
#define VULKAN_HPP_DISABLE_ENHANCED_MODE
#define VULKAN_HPP_NO_SMART_HANDLE
#include "vulkan.hpp"

// Cleanup
#undef Yield
#undef GetClassName
#undef interface
#undef UpdateResource
#undef Always
#undef max
#undef min

namespace vk {
    struct CheckedResult {
        CheckedResult() = default;
        CheckedResult(const CheckedResult&) = default;
        CheckedResult(Result result) : result(result) {

        }
        CheckedResult(VkResult result) : result((Result)result) {

        }

        operator bool() const {
            return result != Result::eSuccess;
        }

        operator Result() const {
            return result;
        }

        Result result;
    };
}

#undef interface
#undef UpdateResource