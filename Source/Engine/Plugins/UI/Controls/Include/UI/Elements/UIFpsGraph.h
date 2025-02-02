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
    class UIImage;
    class UIGraph;

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIFpsGraph : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIFpsGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Update
        /// \param delta
        void Update(double delta);

        /// Overrides
        void StaticInitialize() override;

    private:
        Game::ActorCallback callback;

        UInt32 count;
        UIGraph* graph;
        double avgDelta = 0;
        double lastDelta = 0;
        Game::IGameSystem* gameSystem;
    };
}