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

#include <Game/IWorldSystem.h>

namespace Djinn::Graphics {
    class PacketBatch;
    class IPacketBatcher;
    struct WorldPacket;

    class IPacketSystem : public Game::IWorldSystem {
    public:
        DJINN_COM_INTERFACE();

        IPacketSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldSystem(registry, outer, host) {

        }

        /// Register a batcher
        /// \param batcher
        /// \return
        virtual COM::Result Register(IPacketBatcher* batcher) = 0;

        /// Deregister a batcher
        /// \param batcher
        /// \return
        virtual COM::Result Deregister(IPacketBatcher* batcher) = 0;

        /// Pool world packet
        /// \return
        virtual bool PoolPacket(WorldPacket** out) = 0;

        /// Overrides
        virtual COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case ISystem::kIID:
                    *out = static_cast<IStepSystem*>(this);
                    return COM::kOK;
                case IStepSystem::kIID:
                    *out = static_cast<IStepSystem*>(this);
                    return COM::kOK;
                case Game::IWorldSystem::kIID:
                    *out = static_cast<Game::IWorldSystem*>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IPacketSystem*>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}