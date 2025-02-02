//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/22/2018.
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