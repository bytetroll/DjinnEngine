//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IControl.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::UI {
    class UIStack;

    ClassAttr(UIML::Class("treeview"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UITreeView : public IControl {
    public:
        DJINN_COM_CLASS();

        UITreeView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

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

    private:
        UIStack* stack;
    };
}