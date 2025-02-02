//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "IMainLayout.h"

namespace Djinn::UI {
    class IUICanvas;
}

namespace Djinn::Editor {
    class ILevelLayoutScene;

    class MainLayout : public IMainLayout {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        MainLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        COM::Result AddLevel(ILevel *level) override;

    private:
        struct LevelInstance {
            ILevel* level;
            UI::IUICanvas* canvas;
        };

        Core::Array<LevelInstance> instances;
    };
}