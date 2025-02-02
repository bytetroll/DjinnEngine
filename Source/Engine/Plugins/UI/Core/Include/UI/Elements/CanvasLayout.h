//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>

namespace Djinn::UI {
    class DJINN_EXPORT_PLUGINS_UI_CORE CanvasLayout : public IUIElement {
    public:
        DJINN_COM_CLASS();

        CanvasLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Add a child
        /// \param child
        void AddChild(IUITransformComponent *child);

        /// Remove a child
        /// \param child
        void RemoveChild(IUITransformComponent *child);
    };
}