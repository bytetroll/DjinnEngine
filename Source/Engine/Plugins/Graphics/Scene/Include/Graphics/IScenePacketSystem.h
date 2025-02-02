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
 on 3/1/2018.
//
#pragma once

#include <Graphics/IPacketBatcher.h>

namespace Djinn::Graphics {
    class IScenePacketBatcher;

    class IScenePacketSystem : public COM::TUnknown<IScenePacketSystem>, public IPacketBatcher {
    public:
        DJINN_COM_INTERFACE();

        IScenePacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a batcher
        /// \param batcher
        /// \return
        virtual COM::Result Register(IScenePacketBatcher* batcher) = 0;

        /// Deregister a batcher
        /// \param batcher
        /// \return
        virtual COM::Result Deregister(IScenePacketBatcher* batcher) = 0;
    };
}