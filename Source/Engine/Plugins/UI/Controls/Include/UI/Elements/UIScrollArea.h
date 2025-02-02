//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/UIMLAnnotation.h>
#include <Game/ActorCallback.h>

namespace Djinn::UI {
    class UILayoutTransformComponent;
    class UIButton;
    class UIDiv;

    ClassAttr(UIML::Class("scroll-area"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIScrollArea : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIScrollArea(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Add a child
        /// \param child
        Attr(UIML::SocketAttach())
        void SetContent(IUITransformComponent *child);

    private:
        void ScrollbarUpdated();
        void ContentUpdated();

    private:
        UIButton* scrollbar;
        UIDiv* contentArea;
        Core::WeakPtr<IUITransformComponent> content;
    };
}