//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <UI/UIMLAnnotation.h>
#include <UI/Elements/IDockable.h>

namespace Djinn::UI {
    class UIImage;

    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UIDockingFitter : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIDockingFitter(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;

    public:
        void SetDockable(IDockable* dockable);

    private:
        UIImage* mergeImage;
        UIImage* topImage;
        UIImage* leftImage;
        UIImage* bottomImage;
        UIImage* rightImage;

        EDockingSlotSet slots;
    };
}