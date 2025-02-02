//
// Created by Dev on 12/26/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include "PassBuilder.h"

namespace Djinn::Graphics {
    class IPass : public COM::TUnknown<IPass> {
    public:
        DJINN_COM_INTERFACE();

        IPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Preprocess, called before all builds
        /// \param builder
        /// \return
        virtual COM::Result Preprocess(PassBuilder builder) = 0;

        /// Build all commands
        /// \param builder
        /// \return
        virtual COM::Result Build(PassBuilder builder, double delta) = 0;

        /// Configure this pass
        /// \return
        virtual COM::Result Configure() = 0;
    };
}