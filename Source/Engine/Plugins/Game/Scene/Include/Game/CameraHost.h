//
// Created by Dev on 3/2/2018.
//
#pragma once

#include <Core/Array.h>
#include "ICameraHost.h"

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE CameraHost : public ICameraHost {
    public:
        DJINN_COM_CLASS();

        CameraHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this worldHost
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result Register(ICamera* camera) override;
        COM::Result Deregister(ICamera* camera) override;
        Core::ArrayView<ICamera *> GetCameras() override;

    private:
        Core::Array<ICamera *> batchers;
    };
}