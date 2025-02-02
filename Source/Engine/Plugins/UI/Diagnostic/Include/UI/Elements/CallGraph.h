//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Math/Vector2.h>
#include <Game/ActorCallback.h>
#include <UI/ElementComponents/IUIRenderComponent.h>
#include <Data/Asset.h>
#include <UI/IntermediateCache.h>

namespace Djinn::Game {
    class IGameSystem;
}

namespace Djinn::Async {
    class CallGraph;
}

namespace Djinn::Data {
    class IFontAsset;
}

namespace Djinn::UI {
    class UILabel;

    class DJINN_EXPORT_PLUGINS_UI_DIAGNOSTIC CallGraph : public IUIElement {
    public:
        DJINN_COM_CLASS();

        CallGraph(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Update
        /// \param delta
        void Update(double delta);

        /// Overrides
        void StaticInitialize() override;

    public:
        class Renderer : public IUIRenderComponent {
        public:
            DJINN_COM_CLASS();

            Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

            void Build(Graphics::UIDrawPacketBuffer *buffer) override;

            /// Current font
            Data::Asset<Data::IFontAsset> font = nullptr;

            IntermediateCache cache;

            Core::String labelCache;

            /// Systems
            Game::IGameSystem* system;
        };

    private:
        Renderer *renderer;
    };
}