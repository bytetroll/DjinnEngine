//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Math/Vector2.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::UI {
    ClassAttr(UIML::Class("menuitem"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UIMenuItem : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIMenuItem(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Add an item to this bar
        /// \param element
        void AddItem(IUIElement* element);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Set the title
        /// \param title
        Attr(UIML::Setter("title"))
        void SetTitle(const Core::StringView& title);

    private:
    };
}