//
// Created by Dev on 4/8/2018.
//
#pragma once

#include "IOutputPass.h"

namespace Djinn::Graphics {
    class IOutput;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_OUTPUT OutputPass : public IOutputPass {
    public:
        DJINN_COM_CLASS();

        OutputPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this worldHost
        /// \param pass
        /// \return
        COM::Result Initialize();

        /// Set the current output
        /// \param output
        /// \return
        COM::Result SetOutput(IOutput *output);

        /// Register output
        /// \param output
        /// \return
        COM::Result Register(IOutput* output);

        /// Deregister output
        /// \param output
        /// \return
        COM::Result Deregister(IOutput* output);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        IGraphicsSystem *system = nullptr;
        IOutput *output = nullptr;
        Core::Array<IOutput *> outputs;
    };
}