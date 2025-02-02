//
// Created by Dev on 8/23/2018.
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