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

#include <UI/IUIElement.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>
#include <Core/Annotation.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::UI {
    class IUITransformComponent;
    class StackSeperator;

    ClassAttr(UIML::Class("stack"))
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIStack : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIStack(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

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

        /// Remove all children
        void RemoveChilden();

        /// Set padding
        /// \param padding
        Attr(UIML::Setter("padding"))
        void SetPadding(float padding) {
            static_cast<UIStackTransformComponent *>(GetTransform())->SetPadding(padding);
        }

        /// Get padding
        /// \return
        Attr(UIML::Getter("padding"))
        float GetPadding() const {
            return static_cast<UIStackTransformComponent *>(GetTransform())->GetPadding();
        }

        /// Set padding
        /// \param padding
        Attr(UIML::Setter("mode"))
        void SetMode(UIStackMode mode) {
            static_cast<UIStackTransformComponent *>(GetTransform())->SetMode(mode);
        }

        /// Get padding
        /// \return
        Attr(UIML::Getter("mode"))
        UIStackMode GetMode() const {
            return static_cast<UIStackTransformComponent *>(GetTransform())->GetMode();
        }

        /// Set padding
        /// \param padding
        Attr(UIML::Setter("resizable"))
        void SetResizable(bool state);

        /// Get padding
        /// \return
        Attr(UIML::Getter("resizable"))
        bool IsResizable() const {
            return resizable;
        }

        /// Is this stack empty?
        /// \return
        bool IsEmpty() const {
            return childData.Empty();
        }

    private:
        struct ChildData {
            Core::WeakPtr<StackSeperator> leftSeperator;
            Core::WeakPtr<IUITransformComponent> child;
        };

        Core::Array<ChildData> childData;
        bool resizable = false;
    };
}