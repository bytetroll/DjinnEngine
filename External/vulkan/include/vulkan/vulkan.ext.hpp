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