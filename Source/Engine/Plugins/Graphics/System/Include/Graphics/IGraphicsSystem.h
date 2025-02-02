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
 on 1/10/2018.
//
#pragma once

#include <Game/IStepSystem.h>

namespace Djinn::GHA {
    class IInstance;
    class IDevice;
    class ISwapchain;
    class IConfiguration;
    class IScheduler;
    class IInterpreter;
    class IResourceStager;
}

namespace Djinn::Platform {
    class Window;
}

namespace Djinn::Graphics {
    class IGraph;
    class PacketBatch;

    /// Common Results
    DJINN_COM_RESULT(NoAdapters, true, "Graphics");

    class DJINN_EXPORT_PLUGINS_GRAPHICS_SYSTEM IGraphicsSystem : public Game::IStepSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IGraphicsSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IStepSystem(registry, outer, host) {

        }

        /// Configure this graphics system
        /// \param configuration
        /// \return
        using Djinn::Base::ISystem::Configure;
        virtual COM::Result Configure(GHA::IConfiguration* configuration, Platform::Window* window) = 0;

        /// Get graph
        /// \return
        virtual IGraph* GetGraph() = 0;

        /// Get configuration
        /// \return
        virtual GHA::IConfiguration* GetConfiguration() = 0;

        /// Get scheduler
        /// \return
        virtual GHA::IScheduler* GetScheduler() = 0;

        /// Get interpreter
        /// \return
        virtual GHA::IInterpreter* GetInterpreter() = 0;

        /// Get instance
        /// \return
        virtual GHA::IInstance* GetInstance()  = 0;

        /// Get device
        /// \return
        virtual GHA::IDevice* GetDevice()  = 0;

        /// Get swapchain
        /// \return
        virtual GHA::ISwapchain* GetSwapchain()  = 0;
    };
}