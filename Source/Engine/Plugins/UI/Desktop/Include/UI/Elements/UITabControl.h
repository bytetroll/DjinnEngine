//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/UIMLAnnotation.h>

namespace Djinn::UI {
    ClassAttr(UIML::Class("tabcontrol"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UITabControl : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UITabControl(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Add an item to this tab control
        /// \param element
        void AddItem(IUIElement* element);

        /// Overrides
        void StaticInitialize() override;

    private:
    };
}