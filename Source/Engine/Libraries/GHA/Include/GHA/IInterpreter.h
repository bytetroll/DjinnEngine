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
    class IScheduler;
    class IInstance;
    class IDevice;
    class ISwapchain;
    class IAdapter;

    class IInterpreter : public COM::TUnknown<IInterpreter> {
    public:
        DJINN_COM_INTERFACE();

        IInterpreter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Initialize this interpreter
        /// \return
        virtual COM::Result Initialize(IInstance* instance, IAdapter* adapter, IDevice* device, ISwapchain* swapchain) = 0;

        /// Compile from a scheduler
        /// \param scheduler - Must be compatible
        /// \return
        virtual COM::Result Compile(IScheduler* scheduler) = 0;

        /// Get gpu delta time
        /// \return
        virtual double GetGPUDelta() = 0;
    };
}