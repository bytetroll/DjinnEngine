//
// Created by Dev on 3/1/2018.
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