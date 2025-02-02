//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/IUILayer.h>
#include <Math/Vector2.h>
#include <UI/UIMLAnnotation.h>
#include <Game/InputEvent.h>
#include <UI/UIInputActionBuffer.h>
#include "IDockable.h"

namespace Djinn::UI {
    class UILayoutTransformComponent;
    class UIDocking;
    class UILabel;

    ClassAttr(UIML::Class("window"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UIWindow : public IUIElement, public IDockable {
    public:
        DJINN_COM_CLASS();

        UIWindow(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        IUILayer *GetLayer() override;
        Core::ArrayView<IDockable *> GetDockables() override;
        IUIElement *GetDockingElement() override;
        EDockingSlotSet GetDockingSlots() override;
        void AddDockable(EDockingSlot slot, IDockable *dockable) override;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown *>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance *>(this);
                    return COM::kOK;
                case IActor::kIID:
                    *out = static_cast<IActor *>(this);
                    return COM::kOK;
                case IUIElement::kIID:
                    *out = static_cast<IUIElement *>(this);
                    return COM::kOK;
                case IDockable::kIID:
                    *out = static_cast<IDockable *>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Add a child
        /// \param child
        Attr(UIML::SocketAttach())
        void AddChild(IUITransformComponent *child);

        /// Set the title
        /// \param title
        Attr(UIML::Setter("title"))
        void SetTitle(const Core::StringView& title);

        /// Set the scale mode
        /// \param title
        Attr(UIML::Setter("scale"))
        void SetScale(bool scale);

        /// Maximize this window
        void Maximize();

        /// Minimize this window
        void Minimize();

    private:
        void OnDragged(Game::InputEvent::MouseMoved event, UIInputActionBuffer *actions);

    private:
        UILabel* title = nullptr;
        UILayoutTransformComponent* socket = nullptr;

        bool maximized = false;
        Math::Vector2 minimizedSize;
    };
}