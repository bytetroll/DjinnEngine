//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "IPropertiesHost.h"
#include <Core/Array.h>

namespace Djinn::Editor {
    class PropertiesHost : public IPropertiesHost {
    public:
        DJINN_COM_CLASS();

        PropertiesHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IPropertiesHost(registry, outer, host) {

        }

        /// Overrides
        COM::Result Register(IProperties* properties) override;
        COM::Result Deregister(IProperties* properties) override;

    private:
        Core::Array<Core::WeakPtr<IProperties>> instances;
    };
}