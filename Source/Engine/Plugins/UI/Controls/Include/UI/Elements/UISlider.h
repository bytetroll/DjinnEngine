//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Core/Delegate.h>
#include <Game/ActorEvent.h>

namespace Djinn::UI {
    /// Delegates
    using UISliderChangedDelegate = Core::Delegate<void(float value)>;

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UISlider : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UISlider(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Events
        Game::ActorEvent<UISliderChangedDelegate> pressed;
    };
}