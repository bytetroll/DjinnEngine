#pragma once

#include <Core/Optional.h>
#include <Core/TypeAssignable.h>
#include <Platform/WindowHandle.h>
#include "Format.h"
#include "SwapMode.h"
#include "Size.h"

namespace Djinn {
    namespace GHA {
        struct SwapchainDesc {
            SwapchainDesc& Format(GHA::Format value) {
                format = value;
                return *this;
            }

            SwapchainDesc& SwapMode(GHA::SwapMode value) {
                swapMode = value;
                return *this;
            }

            SwapchainDesc& BackbufferCount(UInt32 value) {
                backbufferCount = value;
                return *this;
            }

            SwapchainDesc& Dimensions(Size value) {
                dimensions = value;
                return *this;
            }

            SwapchainDesc& WindowHandle(Platform::WindowHandle value) {
                windowHandle = value;
                return *this;
            }

            // Backbuffer format
            // May be ignored on special occasions (Like my birthday!)
            Core::Optional<GHA::Format> format;

            // Swap mode
            GHA::SwapMode swapMode = GHA::SwapMode::eMailbox;

            // Backbuffer count, defaulted to max
            Core::Optional<UInt32> backbufferCount;

            // Surface dimensions, defaulted to native screen
            Core::Optional<Size> dimensions;

            // Window handle
            Platform::WindowHandle windowHandle;
        };
    }
}