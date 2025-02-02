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
#include "IGameSystem.h"

namespace Djinn::Async {
    class CallGraph;
}

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SYSTEM GameSystem : public IGameSystem {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        GameSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \return
        COM::Result Initialize();

        /// Step fixed time step
        /// \param step
        void SetFixedStep(double step);

        /// Overrides
        void FlagExit() override;
        COM::Result Register(IStepSystem *system) override;
        COM::Result Deregister(IStepSystem *system) override;
        virtual COM::Result Run() override;
        virtual void SetMaxIPS(UInt32 ips) override;
        virtual double GetDelta() override;
        virtual UInt32 GetMaxIPS() override;
        virtual void EnableAsyncRecording(bool b) override;
        virtual Async::CallGraph *GetAsyncGraph() override;
        virtual COM::Result Configure(Base::PipelineBuilder& builder) override;

    public:
        // All systems
        Core::Array<IStepSystem *> systems;

        // PlatformIndepentTimer
        Core::Timer timer;

        /// Fixed time step
        double fixedTimeStep = 0.01;

        /// Elapsed time
        double elapsedTime = .0;

        /// Accumulated time
        double accumulator = .0;

        /// Current IPS cap
        UInt32 maxIPS = 200;

        /// Delta
        Core::Atomic<double> delta;

        /// Exit flag
        Core::Atomic<bool> exitFlag{false};

        /// Async recording?
        bool isAsyncRecording = false;

        /// Iteration counter
        Core::AtomicCounter iterationCounter;

        /// Async call graphs
        Async::CallGraph *asyncCallgraphPrimary;
        Async::CallGraph *asyncCallgraphSecondary;
    };
}