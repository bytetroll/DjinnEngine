//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Math/Vector2.h>
#include <Game/ActorCallback.h>

namespace Djinn::GHA {
    class IInterpreter;
}

namespace Djinn::Game {
    class IGameSystem;
}

namespace Djinn::UI {
    class UILabel;

    class DJINN_EXPORT_PLUGINS_UI_DIAGNOSTIC UICGTime : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UICGTime(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Update
        /// \param delta
        void Update(double delta);

        /// Overrides
        void StaticInitialize() override;

    private:
        Game::ActorCallback callback;

        UILabel* label = nullptr;

        /// Systems
        Game::IGameSystem* system;
        GHA::IInterpreter* interpreter;

        double avgCDelta = 0;
        double avgGDelta = 0;
        double elapsed = 0;
    };
}