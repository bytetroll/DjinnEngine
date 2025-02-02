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
 on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include "ICommandProcessor.h"

namespace Djinn::Host {
    class IPipeline;
}

namespace Djinn::Game {
    class IWorld;
}

namespace Djinn::Editor {
    class ICommandHost;
    class IViewportCamera;

    class ILevel : public COM::TUnknown<ILevel>, public ICommandProcessor {
    public:
        DJINN_COM_INTERFACE();

        ILevel(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get the game world
        /// \return
        virtual Game::IWorld* GetGameWorld() = 0;

        /// Get the internally hosted pipeline
        /// \return
        virtual Host::IPipeline *GetPipeline() = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case ILevel::kIID:
                    *out = static_cast<ILevel*>(this);
                    return COM::kOK;
                case ICommandProcessor::kIID:
                    *out = static_cast<ICommandProcessor*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}