//
// Created by Dev on 6/28/2018.
//
#pragma once

#include "ITemplatesHost.h"
#include <Core/Array.h>

namespace Djinn::Editor {
    class TemplatesHost : public ITemplatesHost {
    public:
        DJINN_COM_CLASS();

        TemplatesHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITemplatesHost(registry, outer, host) {

        }

        /// Overrides
        COM::Result Register(ITemplates* templates) override;
        COM::Result Deregister(ITemplates* templates) override;

    private:
        Core::Array<Core::WeakPtr<ITemplates>> instances;
    };
}