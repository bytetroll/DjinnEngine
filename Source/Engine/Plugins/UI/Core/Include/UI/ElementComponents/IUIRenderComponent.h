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
 on 8/20/2018.
//
#pragma once

#include <UI/IUIElementComponent.h>

namespace Djinn::Graphics {
    class UIDrawPacketBuffer;
}

namespace Djinn::UI {
    /// The rendering policy of a ui element
    /// A render is always invoked if an area is invalidated regardless of the rendering policy
    enum class UIRenderPolicy {
        /// Always render the element
                eAlways,

        /// Only rendered when the transform has changed
                eTransformChanged,
    };

    class DJINN_EXPORT_PLUGINS_UI_CORE IUIRenderComponent : public IUIElementComponent {
    public:
        DJINN_COM_INTERFACE();

        IUIRenderComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElementComponent(registry, outer, host) {

        }

        /// Build this render component
        /// \param batch
        virtual void Build(Graphics::UIDrawPacketBuffer *buffer) = 0;

        /// Overrides
        void Showed(IUILayer *layer) override;
        void Hidden(IUILayer *layer) override;
        void Destroyed() override;

    public:
        /// Set the rendering policy
        /// \param policy
        void SetRenderPolicy(UIRenderPolicy policy);

        /// Get the rendering policy
        /// \return
        UIRenderPolicy GetRenderPolicy() const {
            return policy;
        }

        /// Mark this render component as invalidated
        void MarkAsInvalidated() {
            isInvalidated = true;
        }

        /// Is this component invalidated?
        /// \return
        bool IsInvalidated() const {
            return isInvalidated;
        }

    protected:
        /// Is this renderer invalidated?
        bool isInvalidated = false;

        /// The render policy
        UIRenderPolicy policy = UIRenderPolicy::eTransformChanged;

    private:
        IUILayer* registeredLayer = nullptr;
    };
}