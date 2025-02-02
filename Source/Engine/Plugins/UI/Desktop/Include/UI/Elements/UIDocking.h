//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <UI/IUIElement.h>
#include <Math/Vector2.h>
#include <UI/UIMLAnnotation.h>
#include <Reflection/Attributes/Proxy.h>
#include "IDockable.h"

namespace Djinn::UI {
    class UIStack;
    class UIWindow;
    class UIDockingManager;

    ClassAttr(
            EnumProxy("eHorizontal", "horizontal"),
            EnumProxy("eVertical", "vertical")
    )
    enum class UIDockingMode {
        eHorizontal,
        eVertical
    };

    ClassAttr(UIML::Class("docking"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UIDocking : public IUIElement, public IDockable {
    public:
        DJINN_COM_CLASS();

        UIDocking(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void StaticInitialize() override;
        IUILayer *GetLayer() override;
        Core::ArrayView<IDockable *> GetDockables() override;
        IUIElement *GetDockingElement() override;
        EDockingSlotSet GetDockingSlots() override;
        void AddDockable(EDockingSlot slot, IDockable *dockable) override;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown *>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance *>(this);
                    return COM::kOK;
                case IActor::kIID:
                    *out = static_cast<IActor *>(this);
                    return COM::kOK;
                case IUIElement::kIID:
                    *out = static_cast<IUIElement *>(this);
                    return COM::kOK;
                case IDockable::kIID:
                    *out = static_cast<IDockable *>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Add a child
        /// \param child
        Attr(UIML::SocketAttach())
        void AddChild(IUITransformComponent *panel);

        /// Remove a child
        /// \param child
        Attr(UIML::SocketDetach())
        void RemoveChild(IUITransformComponent *panel);

        /// Set padding
        /// \param padding
        Attr(UIML::Setter("mode"))
        void SetMode(UIDockingMode mode);

        /// Set the scale mode
        /// \param title
        Attr(UIML::Setter("scale"))
        void SetScale(bool scale);

        /// Set the merged mode
        /// \param title
        Attr(UIML::Setter("merged"))
        void SetMerged(bool merged);

    private:
        Core::Array<IDockable*> dockables;

        UIDockingMode mode = UIDockingMode::eHorizontal;

        UIStack* stack = nullptr;
    };
}