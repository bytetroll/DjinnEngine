//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IControl.h>
#include <UI/UIMLAnnotation.h>
#include <UI/UIMLEvent.h>
#include <UI/Elements/UILabel.h>

namespace Djinn::UI {
    class UIStack;

    using UITreeItemExpandedDelegate = Core::Delegate<void()>;
    using UITreeItemClickedDelegate = Core::Delegate<void()>;

    ClassAttr(UIML::Class("treeitem"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UITreeItem : public IControl {
    public:
        DJINN_COM_CLASS();

        UITreeItem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Set the item text
        /// \param text
        Attr(UIML::Setter("text"))
        void SetText(const Core::StringView &text);

        /// Add a child
        /// \param child
        Attr(UIML::SocketAttach())
        void AddChild(IUITransformComponent *child);

        /// Remove a child
        /// \param child
        Attr(UIML::SocketDetach())
        void RemoveChild(IUITransformComponent *child);

    public:
        /// Events
        DJINN_UIML_EVENT("expanded", BindExpanded, UITreeItemExpandedDelegate, expanded);
        DJINN_UIML_EVENT("clicked", BindClicked, UITreeItemClickedDelegate, clicked);

    public:
        void OnExpanded();
        void OnClicked();

    private:
        UIStack* stack;
        UILabel* label;
    };
}