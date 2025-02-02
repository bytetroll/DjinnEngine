//
// Created by Dev on 8/20/2018.
//
#pragma once

#include <UI/IUIElementComponent.h>
#include <Game/InputEvent.h>
#include <Core/Delegate.h>
#include <Game/ActorEvent.h>

namespace Djinn::UI {
    class IUICanvas;
    class UIInputActionBuffer;

    struct DragEvent {
        Math::Vector2 delta;
    };

    /// Delegate types
    using UIPropagatedEvent = Core::Delegate<void(Game::InputEvent event, IUIElement* element, UIInputActionBuffer *actions)>;
    using UIBeginHover = Core::Delegate<void(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions)>;
    using UIEndHover = Core::Delegate<void(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions)>;
    using UIMouseMoved = Core::Delegate<void(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions)>;
    using UIMousePressed = Core::Delegate<void(Game::InputEvent::MousePressed event, UIInputActionBuffer *actions)>;
    using UIMouseClicked = Core::Delegate<void(Game::InputEvent::MousePressed event, UIInputActionBuffer *actions)>;
    using UIMouseReleased = Core::Delegate<void(Game::InputEvent::MouseReleased event, IUIElement* element, UIInputActionBuffer *actions)>;
    using UIKeyPressed = Core::Delegate<void(Game::InputEvent::KeyPressed event, UIInputActionBuffer *actions)>;
    using UIKeyDown = Core::Delegate<void(Game::InputEvent::KeyDown event, UIInputActionBuffer *actions)>;
    using UIKeyReleased = Core::Delegate<void(Game::InputEvent::KeyReleased event, UIInputActionBuffer *actions)>;
    using UIMouseDoubleClicked = Core::Delegate<void(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions)>;
    using UICharacterTyped = Core::Delegate<void(Game::InputEvent::Character event, UIInputActionBuffer *actions)>;
    using UIDragged = Core::Delegate<void(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions)>;

    class DJINN_EXPORT_PLUGINS_UI_CORE UIInputComponent : public IUIElementComponent {
    public:
        DJINN_COM_CLASS();

        UIInputComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

    public:
        /// Bind a dragged event
        /// \param event
        void BindDragged(IActorSystemInstance* instance, const UIDragged& draggedEvent);

    public:
        /// Dispatchers
        Game::ActorEvent<UIPropagatedEvent> propagated;
        Game::ActorEvent<UICharacterTyped> characterTyped;
        Game::ActorEvent<UIBeginHover> beginHover;
        Game::ActorEvent<UIEndHover> endHover;
        Game::ActorEvent<UIMouseMoved> mouseMoved;
        Game::ActorEvent<UIMouseClicked> mouseClicked;
        Game::ActorEvent<UIMousePressed> mousePressed;
        Game::ActorEvent<UIMouseReleased> mouseReleased;
        Game::ActorEvent<UIKeyPressed> keyPressed;
        Game::ActorEvent<UIKeyDown> keyDown;
        Game::ActorEvent<UIKeyReleased> keyReleased;
        Game::ActorEvent<UIMouseDoubleClicked> mouseDoubleClicked;

    public:
        /// \return
        /// Is this input component being dragged?
        bool IsDragging() const {
            return isDragging;
        }

    private:
        void EnsureDragHandles();

        bool dragHandles = false;
        bool isDragging = false;
    };
}