//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Editor {
    class IHierarchy;
    class ILevel;

    class IHierarchyHost : public COM::TUnknown<IHierarchyHost> {
    public:
        DJINN_COM_INTERFACE();

        IHierarchyHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a Hierarchy
        /// \param Hierarchy
        /// \return
        virtual COM::Result Register(IHierarchy* Hierarchy) = 0;

        /// Deregister a Hierarchy
        /// \param Hierarchy
        /// \return
        virtual COM::Result Deregister(IHierarchy* Hierarchy) = 0;
    };
}