//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "ILevelHost.h"

namespace Djinn::Editor {
    class Level;

    class LevelHost : public ILevelHost {
    public:
        DJINN_COM_CLASS();

        LevelHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this world worldHost
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result CreateLevel(ILevel **out) override;
        COM::Result SaveLevel(ILevelAsset **out) override;
        COM::Result LoadLevel(ILevelAsset *asset) override;
    };
}