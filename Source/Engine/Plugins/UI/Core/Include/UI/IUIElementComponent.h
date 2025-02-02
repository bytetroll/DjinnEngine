//
// Created by Dev on 8/19/2018.
//
#pragma once

#include <Game/IActorComponent.h>
#include "Style.h"

namespace Djinn::UI {
    class IUIElement;
    class IUILayer;

    class DJINN_EXPORT_PLUGINS_UI_CORE IUIElementComponent : public Game::IActorComponent {
    public:
        DJINN_COM_INTERFACE();

        IUIElementComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IActorComponent(registry, outer, host) {

        }

        /// Get the owning element
        /// \return
        IUIElement *GetElement() const;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown *>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance *>(this);
                    return COM::kOK;
                case IActorComponent::kIID:
                    *out = static_cast<IActorComponent *>(this);
                    return COM::kOK;
                case IUIElementComponent::kIID:
                    *out = static_cast<IUIElementComponent *>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Invoked when the element appears
        virtual void Showed(IUILayer *layer) {};

        /// Invoked when the element disappears
        virtual void Hidden(IUILayer *layer) {};

        /// Invoked during styling
        /// Usually invoked after static initialization
        virtual void StyleEvent(const ElementStyle& style) {}
    };
}