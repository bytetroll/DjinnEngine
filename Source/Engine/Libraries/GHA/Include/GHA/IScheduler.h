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
 on 12/21/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::GHA {
    union Command;
    class ScheduledNode;
    class ISwapchain;
    struct SchedulerContext;
    class IDevice;

    class IScheduler : public COM::TUnknown<IScheduler> {
    public:
        DJINN_COM_INTERFACE();

        IScheduler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this scheduler
        /// \return
        virtual COM::Result Initialize(IDevice* device, ISwapchain *swapchain) = 0;

        /// Get scheduler context
        /// \return
        virtual SchedulerContext* GetContext() = 0;

        /// Schedule a batch of nodes
        /// \param count
        /// \param out
        virtual COM::Result Schedule(UInt32 count, ScheduledNode** nodes) = 0;

        /// Compile all scheduled nodes, will flush current scheduled nodes
        /// \return
        virtual COM::Result Compile() = 0;
    };
}