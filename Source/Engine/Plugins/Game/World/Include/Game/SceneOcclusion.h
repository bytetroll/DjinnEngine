//
// Created by Dev on 8/27/2018.
//
#pragma once

#include "ISceneOcclusion.h"

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_WORLD SceneOcclusion : public ISceneOcclusion {
    public:
        DJINN_COM_CLASS();

        SceneOcclusion(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this occlusion
        /// \return
        COM::Result Initialize();
    };
}