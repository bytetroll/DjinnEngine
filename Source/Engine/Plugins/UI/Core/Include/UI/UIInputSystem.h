//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/23/2018.
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
