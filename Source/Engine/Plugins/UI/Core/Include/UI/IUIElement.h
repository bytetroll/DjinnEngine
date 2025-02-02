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

#include <Game/IActor.h>
#include <Core/ArrayView.h>
#include "Style.h"

namespace Djinn::UI {
    class IUITransformComponent;
    class IUIRenderComponent;
    class IUILayer;

    class DJINN_EXPORT_PLUGINS_UI_CORE IUIElement : public Game::IActor {
    public:
        DJINN_COM_INTERFACE();

        IUIElement(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Overrides
        void StaticInitialize() override;

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
            }
            return COM::kNotImplemented;
        }

    public:
        /// Invoked before a transformation update
        /// \return - True if successful caching
        virtual bool Cache() { return true; }

        /// Invoked when the element appears
        virtual void Showed(IUILayer *layer) {};

        /// Invoked when the element disappears
        virtual void Hidden(IUILayer *layer) {};

        /// Invoked during styling
        /// Usually invoked after static initialization
        virtual void StyleEvent(const ElementStyle &style) {}

        /// Invoked during a style change, each element handles their own styling
        /// \param style
        virtual Style *RequestStyle(Style *style);

    public:
        /// Find a child
        /// \param name
        /// \return
        IUIElement *FindChild(const Core::StringView &name);

        /// Helper
        template<typename T>
        T* FindChild(const Core::StringView& name) {
            return COM::Cast<T>(FindChild(name));
        }

        /// Set the transform
        /// \param transform
        void SetTransform(IUITransformComponent *transform);

        /// Get the transform component
        /// \return
        IUITransformComponent *GetTransform() const {
            return transform;
        }

        /// Mark this element as uncached
        void MarkAsUncached();

        /// Does this element needs a recaching?
        /// \return
        bool NeedsCaching() const {
            return needsCaching;
        }

        /// Mark this element as cached
        void MarkAsCached() {
            needsCaching = false;
        }

        /// Mark this element as unstyled
        /// \param state
        void MarkAsUnstyled();

        /// Mark this element as unstyled
        /// \param state
        void MarkAsStyled();

        /// Does this element need styling?
        /// \return
        bool NeedsStyling() const {
            return needsStyling;
        }

        /// Set the state
        /// \param id
        void SetStyleState(const StyleStateID &id);

        /// Remove the current state
        /// \param id
        void RemoveStyleState();

        /// Add a style class
        /// \param id
        void AddStyleClass(const StyleClassID &id);

        /// Remove a style class
        /// \param id
        void RemoveStyleClass(const StyleClassID &id);

        /// Apply a style
        /// \param style
        void ApplyStyle();

        /// Get current states
        /// \return
        StyleStateID GetStyleState() const {
            return styleState;
        }

        /// Get the compiled style
        /// \return
        const ElementStyle &GetElementStyle() const {
            return elementStyle;
        }

        /// Get the style classes
        /// \return
        Core::ArrayView<StyleClassID> GetStyleClasses() {
            return styleClasses;
        }

    private:
        /// Root transform component
        Core::WeakPtr<IUITransformComponent> transform;

        /// Current state
        StyleStateID styleState;

        /// All classes
        Core::StackArray4<StyleClassID> styleClasses;

        /// Compiled style
        ElementStyle elementStyle;

        /// Parent style
        Style* style = nullptr;

        /// Does this element need a recache?
        bool needsCaching = false;

        /// Does this element need a style update?
        bool needsStyling = true;
    };
}