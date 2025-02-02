//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Editor {
    class IViewport;
    class ILevel;

    class IViewportHost : public COM::TUnknown<IViewportHost> {
    public:
        DJINN_COM_INTERFACE();

        IViewportHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a viewport
        /// \param viewport
        /// \return
        virtual COM::Result Register(IViewport* viewport) = 0;

        /// Deregister a viewport
        /// \param viewport
        /// \return
        virtual COM::Result Deregister(IViewport* viewport) = 0;
    };
}