//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "IViewportHost.h"
#include <Core/Array.h>

namespace Djinn::Editor {
    class ViewportHost : public IViewportHost {
    public:
        DJINN_COM_CLASS();

        ViewportHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IViewportHost(registry, outer, host) {

        }

        /// Overrides
        COM::Result Register(IViewport* viewport) override;
        COM::Result Deregister(IViewport* viewport) override;

    private:
        Core::Array<Core::WeakPtr<IViewport>> viewports;
    };
}