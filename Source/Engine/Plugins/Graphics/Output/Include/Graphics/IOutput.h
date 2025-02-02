//
// Created by Dev on 4/8/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Graphics/PassBuilder.h>

namespace Djinn::GHA {
    class ITexture;
}

namespace Djinn::Graphics {
    class IOutput : public COM::TUnknown<IOutput> {
    public:
        DJINN_COM_INTERFACE();

        IOutput(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Configure
        /// \return
        virtual COM::Result Configure(PassBuilder builder) = 0;

        /// Get size of this output
        /// \return
        virtual GHA::Size GetSize() = 0;

        /// Get texture
        /// \return
        virtual Texture GetTexture(PassBuilder builder) = 0;
    };
}