//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Game {
    class IWorld;
    class IScene;
}

namespace Djinn::UI {
    class IUICanvas;
}

namespace Djinn::Editor {
    class IEditorWorld : public COM::TUnknown<IEditorWorld> {
    public:
        DJINN_COM_INTERFACE();

        IEditorWorld(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get the game world
        /// \return
        virtual Game::IWorld* GetGameWorld() = 0;

        /// Get the game canvas
        /// \return
        virtual UI::IUICanvas* GetGameCanvas() = 0;
    };
}