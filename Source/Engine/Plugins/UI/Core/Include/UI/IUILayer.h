//
// Created by Dev on 1/9/2019.
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