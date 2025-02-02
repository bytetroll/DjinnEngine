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
 on 1/9/2019.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/ArrayView.h>
#include <Graphics/IPacketBatcher.h>
#include "UILayerPass.h"

namespace Djinn::Graphics {
    struct UIPacket;
}

namespace Djinn::UI {
    class IUIRenderComponent;
    class IUITransformComponent;

    class IUILayer {
    public:
        DJINN_COM_INTERFACEID();

        /// Register a render component
        /// \param component
        virtual void Register(IUIRenderComponent* component) = 0;

        /// Deregister component
        /// \param component
        virtual void Deregister(IUIRenderComponent* component) = 0;

        /// Register a layer
        /// \param component
        virtual void Register(IUILayer *layer) = 0;

        /// Deregister layer
        /// \param component
        virtual void Deregister(IUILayer *layer) = 0;

        /// Build onto the supplied ui packet
        /// \param batch
        /// \return
        virtual COM::Result Build(Graphics::UIPacket* packet, UILayerPass pass) = 0;
    };
}