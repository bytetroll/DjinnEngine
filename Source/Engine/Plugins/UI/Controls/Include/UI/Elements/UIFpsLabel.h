//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Game/ActorCallback.h>

namespace Djinn::Game {
    class IGameSystem;
}

namespace Djinn::UI {
    class UILabel;

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIFpsLabel : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIFpsLabel(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Update
        /// \param delta
        void Update(double delta);

        /// Overrides
        void StaticInitialize() override;

    private:
        Game::ActorCallback callback;

        UILabel* label;
        double avgDelta = 0;
        double elapsed = 0;
        Game::IGameSystem* gameSystem;
    };
}