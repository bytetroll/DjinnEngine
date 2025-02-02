//
// Created by Dev on 4/8/2018.
//
#pragma once

#include "IOutputHost.h"

namespace Djinn::Graphics {
    class OutputPass;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_OUTPUT OutputHost : public IOutputHost {
    public:
        DJINN_COM_CLASS();

        OutputHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this worldHost
        /// \param pass
        /// \return
        COM::Result Initialize(OutputPass* pass);

        /// Overrides
        virtual COM::Result Register(IOutput *output) override;
        virtual COM::Result Deregister(IOutput *output) override;
        virtual COM::Result SetOutput(IOutput *output) override;

    private:
        OutputPass* pass;
    };
}