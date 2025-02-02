//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Editor {
    class ITemplates;
    class ILevel;

    class ITemplatesHost : public COM::TUnknown<ITemplatesHost> {
    public:
        DJINN_COM_INTERFACE();

        ITemplatesHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a templates
        /// \param templates
        /// \return
        virtual COM::Result Register(ITemplates* templates) = 0;

        /// Deregister a templates
        /// \param templates
        /// \return
        virtual COM::Result Deregister(ITemplates* templates) = 0;
    };
}