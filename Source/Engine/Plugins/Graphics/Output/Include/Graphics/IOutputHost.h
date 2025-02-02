//
// Created by Dev on 4/8/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <GHA/Size.h>
#include <Core/Optional.h>

namespace Djinn::Graphics {
    class IOutput;

    class IOutputHost : public COM::TUnknown<IOutputHost> {
    public:
        DJINN_COM_INTERFACE();

        IOutputHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register an output
        /// \param output
        /// \return
        virtual COM::Result Register(IOutput* output) = 0;

        /// Deregister an output
        /// \param output
        /// \return
        virtual COM::Result Deregister(IOutput* output) = 0;

        /// Set current active output
        /// \param output
        /// \return
        virtual COM::Result SetOutput(IOutput* output) = 0;
    };
}