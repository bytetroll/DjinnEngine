//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <Game/InputEvent.h>
#include <Math/Vector2.h>
#include <Async/Mutex.h>
#include "IUIInputSystem.h"
#include "UIInputActionBuffer.h"

namespace Djinn::Game {
    class IInputSystem;
}

namespace Djinn::UI {
    class IUIElement;

    class DJINN_EXPORT_PLUGINS_UI_CORE UIInputSystem : public IUIInputSystem {
    public:
        DJINN_COM_CLASS();

        UIInputSystem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this system
        /// \return
        COM::Result Initialize();

        /// Overrides
        void Register(IUICanvas *canvas) override;
        void Deregister(IUICanvas *canvas) override;
        COM::Result Configure(Base::PipelineBuilder &builder) override;
        COM::Result Run(Game::StepMode mode, double delta) override;
        Game::StepModeSet GetStepMode() override;

    private:
        void SignalPropagations(IUIElement* element, const Game::InputEvent& event);

        Game::IInputSystem *inputSystem;
        Async::Mutex mutex;

        /// Last canvas
        Core::WeakPtr<IUICanvas> lastCanvas;

        /// Focused input components
        Core::WeakPtr<UIInputComponent> focusedMouseInput;
        Core::WeakPtr<UIInputComponent> focusedKeyboardInput;

        /// Last hoverered entity
        Core::WeakPtr<UIInputComponent> lastHovered;

        /// Current screen space mouse position
        Math::Vector2 screenMousePosition;

        /// Current mouse position
        Math::Vector2 mousePosition;

        /// Last mouse position
        Math::Vector2 lastMousePosition;

        /// Action buffer
        UIInputActionBuffer actionBuffer;

        /// Mouse moved event buffer
        Core::Array<Game::InputEvent::MouseMoved> mouseMovedBuffer;
        Core::Array<Game::InputEvent::MousePressed> mousePressedBuffer;
        Core::Array<Game::InputEvent::MouseReleased> mouseReleasedBuffer;
        Core::Array<Game::InputEvent::Character> characterBuffer;

    private:
        /// All canvases
        Core::Array<Core::WeakPtr<IUICanvas>> canvases;
    };
}
