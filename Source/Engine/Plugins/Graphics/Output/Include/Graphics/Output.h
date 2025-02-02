//
// Created by Dev on 4/8/2018.
//
#pragma once

#include <GHA/Size.h>
#include <Core/Optional.h>
#include <Graphics/PassBuilder.h>
#include <Graphics/ID.h>
#include "IOutput.h"

namespace Djinn::Graphics {
    class DJINN_EXPORT_PLUGINS_GRAPHICS_OUTPUT Output : public IOutput {
    public:
        DJINN_COM_CLASS();

        Output(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this output
        /// \param size
        /// \return
        COM::Result Initialize(const GHA::Size& size);

        /// Overrides
        COM::Result Configure(PassBuilder builder) override;
        GHA::Size GetSize() override;
        Texture GetTexture(PassBuilder builder) override;

    private:
        GHA::ITexture* texture = nullptr;
    };
}