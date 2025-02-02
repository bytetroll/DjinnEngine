//
// Created by Dev on 2/25/2018.
//
#pragma once

#include "IProcess.h"
#include "PassBuilder.h"

namespace Djinn::Graphics {
    class Pipeline;

    class RootProcess : public IProcess {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        RootProcess(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this process
        /// \return
        COM::Result Initialize();

        /// Build this process
        /// \param builder
        /// \param delta
        /// \return
        COM::Result Build(Graphics::PassBuilder builder, double delta);

        /// Overrides
        virtual IPipeline *GetPipeline() override;

    private:
        Pipeline* pipeline = nullptr;
    };
}