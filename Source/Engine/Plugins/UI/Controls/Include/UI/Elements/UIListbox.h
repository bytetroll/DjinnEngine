//
// Created by Dev on 8/23/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Game/ActorCallback.h>

namespace Djinn::UI {
    class IUITransformComponent;
    class UIStackTransformComponent;

    class DJINN_EXPORT_PLUGINS_UI_CONTROLS UIListbox : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIListbox(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Add an item to this listbox
        /// \param transform
        void AddItem(IUITransformComponent* transform);

    private:
        UIStackTransformComponent* items;
    };
}