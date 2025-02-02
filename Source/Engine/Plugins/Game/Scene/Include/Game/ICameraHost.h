//
// Created by Dev on 3/2/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/ArrayView.h>

namespace Djinn::Game {
    class ICamera;

    class DJINN_EXPORT_PLUGINS_GAME_SCENE ICameraHost : public COM::TUnknown<ICameraHost> {
    public:
        DJINN_COM_INTERFACE();

        ICameraHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a camera
        /// \param batcher
        /// \return
        virtual COM::Result Register(ICamera* batcher) = 0;

        /// Deregister a camera
        /// \param batcher
        /// \return
        virtual COM::Result Deregister(ICamera* batcher) = 0;

        /// Get all cameras
        /// \return
        virtual Core::ArrayView<ICamera*> GetCameras() = 0;
    };
}