//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "IHierarchyHost.h"
#include <Core/Array.h>

namespace Djinn::Editor {
    class DJINN_EXPORT_EDITOR_HIERARCHY HierarchyHost : public IHierarchyHost {
    public:
        DJINN_COM_CLASS();

        HierarchyHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IHierarchyHost(registry, outer, host) {

        }

        /// Overrides
        COM::Result Register(IHierarchy* Hierarchy) override;
        COM::Result Deregister(IHierarchy* Hierarchy) override;

    private:
        Core::Array<Core::WeakPtr<IHierarchy>> hierarchies;
    };
}