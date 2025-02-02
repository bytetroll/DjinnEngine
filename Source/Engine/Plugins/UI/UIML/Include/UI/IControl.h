//
// Created by Dev on 9/1/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Base/Paths.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_UIML IControl : public IUIElement {
    public:
        DJINN_COM_INTERFACE();

        IControl(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Construct this control
        /// \param path
        void Construct(const Base::PathID& path);

    public:
        /// Get the uiml path
        /// \return
        const Base::PathID& GetPath() const {
            return path;
        }

        /// Get the layout transformation
        /// \return
        UILayoutTransformComponent* GetLayout() {
            return static_cast<UILayoutTransformComponent*>(GetTransform());
        }

    private:
        Base::PathID path;
    };
}