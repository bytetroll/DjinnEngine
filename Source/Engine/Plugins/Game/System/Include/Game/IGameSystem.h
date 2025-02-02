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

#include <Base/IApplicationSystem.h>

namespace Djinn::Game {
    class IStepSystem;

    class DJINN_EXPORT_PLUGINS_GAME_SYSTEM IGameSystem : public Base::IApplicationSystem {
    public:
        DJINN_COM_INTERFACE();

        /// Construct
        /// \param registry
        /// \param outer
        IGameSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IApplicationSystem(registry, outer, host) {

        }

        /// Register a system
        /// \param system
        /// \return
        virtual COM::Result Register(IStepSystem *system) = 0;

        /// Deregister a system
        /// \param system
        /// \return
        virtual COM::Result Deregister(IStepSystem *system) = 0;

        /// Enable or disable the recording of async calls
        /// \param b
        virtual void EnableAsyncRecording(bool b = true) = 0;

        /// Get the async call graph of the last iteration
        /// \return
        virtual Async::CallGraph* GetAsyncGraph() = 0;

        /// Set maximum iterations per second
        /// \param ips
        virtual void SetMaxIPS(UInt32 ips) = 0;

        /// Get maximum iterations per second
        /// \return
        virtual UInt32 GetMaxIPS() = 0;

        /// Get iteration delta
        /// \return
        virtual double GetDelta() = 0;

        /// Flag for exit
        virtual void FlagExit() = 0;
    };
}