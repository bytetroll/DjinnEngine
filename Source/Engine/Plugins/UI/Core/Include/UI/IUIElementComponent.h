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
 on 8/19/2018.
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