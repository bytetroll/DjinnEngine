//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "IEditorWorld.h"

namespace Djinn::UI {
    class IUICanvas;
}

namespace Djinn::Editor {
    class EditorWorld : public IEditorWorld {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        EditorWorld(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize the editor world
        /// \return
        COM::Result Initialize();

        /// Overrides
        Game::IWorld *GetGameWorld() override;
        UI::IUICanvas *GetGameCanvas() override;

    private:
        Game::IWorld* world = nullptr;
        UI::IUICanvas* canvas = nullptr;
    };
}