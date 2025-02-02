//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::UI {
    ClassAttr(UIML::Class("div"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIDiv : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIDiv(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Add a child
        /// \param child
        Attr(UIML::SocketAttach())
        void AddChild(IUITransformComponent *child);

        /// Remove a child
        /// \param child
        Attr(UIML::SocketDetach())
        void RemoveChild(IUITransformComponent *child);
    };
}