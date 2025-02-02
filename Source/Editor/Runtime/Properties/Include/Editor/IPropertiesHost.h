//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Editor {
    class IProperties;
    class ILevel;

    class IPropertiesHost : public COM::TUnknown<IPropertiesHost> {
    public:
        DJINN_COM_INTERFACE();

        IPropertiesHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a properties
        /// \param properties
        /// \return
        virtual COM::Result Register(IProperties* properties) = 0;

        /// Deregister a properties
        /// \param properties
        /// \return
        virtual COM::Result Deregister(IProperties* properties) = 0;
    };
}