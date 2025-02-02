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
 on 8/22/2018.
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