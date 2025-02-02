//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/ElementComponents/UIStackTransformComponent.h>

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CONTROLS StackSeperator : public IUIElement {
    public:
        DJINN_COM_CLASS();

        StackSeperator(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        void SetMode(UIStackMode mode);

        void SetElements(IUIElement* left, IUIElement* right);

    private:
        IUIElement* left = nullptr;
        IUIElement* right = nullptr;

        UIStackMode mode;
    };
}