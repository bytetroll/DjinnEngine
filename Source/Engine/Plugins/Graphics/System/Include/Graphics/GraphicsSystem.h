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

#include <Core/Timer.h>
#include <Core/Array.h>
#include "IGraphicsSystem.h"

namespace Djinn::Graphics {
    class Graph;
    class PacketBatch;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_SYSTEM GraphicsSystem : public IGraphicsSystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        GraphicsSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Configure(GHA::IConfiguration *configuration, Platform::Window *window) override;
        virtual IGraph *GetGraph() override;
        virtual GHA::IConfiguration *GetConfiguration() override;
        virtual GHA::IScheduler *GetScheduler() override;
        virtual GHA::IInterpreter *GetInterpreter() override;
        virtual COM::Result Run(Game::StepMode mode, double delta) override;
        virtual Game::StepModeSet GetStepMode() override;
        virtual GHA::IInstance *GetInstance() override;
        virtual GHA::IDevice *GetDevice() override;
        virtual GHA::ISwapchain *GetSwapchain() override;
        virtual COM::Result Configure(Base::PipelineBuilder& builder) override;

    private:
        GHA::IConfiguration *configuration = nullptr;
        GHA::IScheduler *scheduler = nullptr;
        GHA::IInterpreter *interpreter = nullptr;
        GHA::IInstance *instance = nullptr;
        GHA::IDevice *device = nullptr;
        GHA::ISwapchain *swapchain = nullptr;
        Platform::Window *window = nullptr;
        Graph *graph = nullptr;
    };
}